#include <iostream>
#include <vector>
#include <map>
#include <queue>
using namespace std;

// saves all the neighbor id of node
// saves  flow size of the node relates to specific destination
// makes the node know next node id to make always be shortest path 
class Node {
public:
  bool is_sdn = false;
  vector<unsigned int> neighbor_id;
  map<unsigned int, int> flow;  // destination , flow_size
  map<unsigned int, unsigned int> router_table;  // destination , next_node
  map<unsigned int, vector< pair<unsigned int, double> > > router_table_sdn;  // destination , < next_node, ratio>
};
Node node[5000];


// Makes every node has a shortest path to destination
// inputs the destination id , and total node number
// returns the path thay every node know what is the next node 
unsigned int* BFS(const unsigned int dst_id,const int nodes) { 

  bool *visited = new bool[nodes];
  unsigned int *next_node = new unsigned int[nodes];  // because BFS begin with destination , so pre node in BFS will be the next node actually in routing table

  for(int i = 0; i < nodes; i++) {
    visited[i] = false;
    next_node[i] = nodes + 2;
  }

  queue<unsigned int> q;
  unsigned int current_node, neighbor;

  visited[dst_id] = true;
  q.push(dst_id);
  next_node[dst_id] = nodes + 3;

  while(!q.empty()) {
    current_node = q.front();
    q.pop();
    int size = node[current_node].neighbor_id.size();
    for(int i = 0; i < size; i++) {  //traverse all the neighbor of current node
      neighbor = node[current_node].neighbor_id[i];
      if(!visited[neighbor]) {
        visited[neighbor] = true;
        next_node[neighbor] = current_node;
        q.push(neighbor);
      }
    }
  }
  delete [] visited;
  return next_node;
}

// Builds routing table by next_node
void Build_Routing_Table(const int nodes,const unsigned int dst_id,const unsigned int * next_node) {
  for(int i = 0; i < nodes; i++) {
    if(next_node[i] != nodes + 3) {  // i is not the destination 
      if(node[i].is_sdn == true) {
        node[i].router_table_sdn[dst_id].push_back(make_pair(next_node[i] , 100));  // because I use basline so only go for one node 
      } else {
        node[i].router_table[dst_id] = next_node[i];  // destination -> next node
      }
    } else {
      if(node[i].is_sdn == true) {
        node[i].router_table_sdn[dst_id].push_back(make_pair(i , 100));  // next node is itself
      } else {
        node[i].router_table[dst_id] = dst_id;
      }
    }
  }
}

// outputs every node router table
// destination , next node
// destination , next node , ratio (dor sdn)
void Output(const int nodes,const int dsts, const vector<unsigned int>& v_dsts) {
  for(int i = 0; i < nodes; i++) {
    cout << i << "\n";
    if(node[i].is_sdn == true) {
      for(int j = 0; j < dsts; j++) {
        cout << v_dsts[j];
        for(int k = 0; k < int(node[i].router_table_sdn[v_dsts[j]].size()); k++) {
          cout << " " << node[i].router_table_sdn[v_dsts[j]][k].first << " " << node[i].router_table_sdn[v_dsts[j]][k].second << "%" ;
        }
        cout << "\n";
      } 
    } else {
      for(int j = 0; j < dsts; j++) {
        cout << v_dsts[j] << " " << node[i].router_table[v_dsts[j]]<< " " <<  "\n";
      }
    }
  }
}
int main() {
  int nodes, sdn_nodes, dsts, links, pairs;  // nodes number, sdn number, destination number, edge number (node conect to another node), nodes number which is with flow 
  cin >> nodes >> sdn_nodes >> dsts >> links >> pairs;
  
  unsigned int sdn_id;
  for(int i = 0; i < sdn_nodes; i++) {
    cin >> sdn_id;
    node[sdn_id].is_sdn = true;
  }

  vector<unsigned int> v_dsts(dsts);
  for(auto &dst_id : v_dsts) cin>>dst_id;

  unsigned int link_id, current_node_id, currtnent_neighbor;
  for(int i = 0; i < links; i++) {
    cin>> link_id >> current_node_id >> currtnent_neighbor; 
    node[current_node_id].neighbor_id.push_back(currtnent_neighbor);
    node[currtnent_neighbor].neighbor_id.push_back(current_node_id); 
  }

  unsigned int flow_id, src, dst; 
  int flow_size;
  for(int i = 0; i < pairs; i++) {
    cin>> flow_id >> src >> dst >> flow_size;
    node[src].flow[dst] = flow_size;
  }

  // makes every node has a router table which path will be shortest path
  unsigned int *temp_pre_node;
  for(int i = 0; i < dsts; i++) {
    temp_pre_node = BFS(v_dsts[i], nodes);
    Build_Routing_Table(nodes, v_dsts[i], temp_pre_node);
  }
  Output(nodes, dsts, v_dsts);
  delete [] temp_pre_node;
  
  return 0;
}