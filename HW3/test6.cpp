#include <iostream>
#include <map>
#include <queue>
#include <utility>
#include <climits>
#include <functional>
#include <iomanip>
#include <stack>
//------------------
#include <algorithm>
// #include <pair>
#include <utility>

using namespace std;

#define SET(func_name,type,var_name,_var_name) void func_name(type _var_name) { var_name = _var_name ;} 
#define GET(func_name,type,var_name) type func_name() const { return var_name ;}

class header;
class payload;
class packet;
class node;
class event;
class link; // new

// for simplicity, we use a const int to simulate the delay
// if you want to simulate the more details, you should revise it to be a class
const unsigned int ONE_HOP_DELAY = 10;
const unsigned int BROCAST_ID = UINT_MAX;

// BROCAST_ID means that all neighbors are receivers; UINT_MAX is the maximum value of unsigned int

class header {
    public:
        virtual ~header() {}

        SET(setSrcID, unsigned int , srcID, _srcID);
        SET(setDstID, unsigned int , dstID, _dstID);
        SET(setPreID, unsigned int , preID, _preID);
        SET(setNexID, unsigned int , nexID, _nexID);
        GET(getSrcID, unsigned int , srcID);
        GET(getDstID, unsigned int , dstID);
        GET(getPreID, unsigned int , preID);
        GET(getNexID, unsigned int , nexID);
        
        virtual string type() = 0;
        
        // factory concept: generate a header
        class header_generator {
                // lock the copy constructor
                header_generator(header_generator &){}
                // store all possible types of header
                static map<string,header_generator*> prototypes;
            protected:
                // allow derived class to use it
                header_generator() {}
                // after you create a new header type, please register the factory of this header type by this function
                void register_header_type(header_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your header
                virtual header* generate() = 0 ;
            public:
                // you have to implement your own type() to return your header type
        	    virtual string type() = 0 ;
        	    // this function is used to generate any type of header derived
        	    static header * generate (string type) {
            		if(prototypes.find(type) != prototypes.end()){ // if this type derived exists 
            			return prototypes[type]->generate(); // generate it!!
            		}
            		std::cerr << "no such header type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered header types: " << endl;
            	    for (map<string,header::header_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~header_generator(){};
        };
        
    protected:
        header():srcID(BROCAST_ID),dstID(BROCAST_ID),preID(BROCAST_ID),nexID(BROCAST_ID){} // this constructor cannot be directly called by users

    private:
        unsigned int srcID;
        unsigned int dstID;
        unsigned int preID;
        unsigned int nexID;
        header(header&){} // this constructor cannot be directly called by users
};
map<string,header::header_generator*> header::header_generator::prototypes;

class TRA_data_header : public header{
        TRA_data_header(TRA_data_header&){} // cannot be called by users
        
    protected:
        TRA_data_header(){} // this constructor cannot be directly called by users

    public:
        ~TRA_data_header(){}
        string type() { return "TRA_data_header"; }

        class TRA_data_header_generator;
        friend class TRA_data_header_generator;
        // TRA_data_header_generator is derived from header_generator to generate a header
        class TRA_data_header_generator : public header_generator{
                static TRA_data_header_generator sample;
                // this constructor is only for sample to register this header type
                TRA_data_header_generator() { /*cout << "TRA_data_header registered" << endl;*/ register_header_type(&sample); }
            protected:
                virtual header * generate(){
                    // cout << "TRA_data_header generated" << endl;
                    return new TRA_data_header; 
                }
            public:
                virtual string type() { return "TRA_data_header";}
                ~TRA_data_header_generator(){}
        
        };
};
TRA_data_header::TRA_data_header_generator TRA_data_header::TRA_data_header_generator::sample;

class TRA_ctrl_header : public header{
        TRA_ctrl_header(TRA_ctrl_header&){} // cannot be called by users
        
    protected:
        TRA_ctrl_header(){} // this constructor cannot be directly called by users

    public:
        ~TRA_ctrl_header(){}
        string type() { return "TRA_ctrl_header"; }

        class TRA_ctrl_header_generator;
        friend class TRA_ctrl_header_generator;
        // TRA_ctrl_header_generator is derived from header_generator to generate a header
        class TRA_ctrl_header_generator : public header_generator{
                static TRA_ctrl_header_generator sample;
                // this constructor is only for sample to register this header type
                TRA_ctrl_header_generator() { /*cout << "TRA_ctrl_header registered" << endl;*/ register_header_type(&sample); }
            protected:
                virtual header * generate(){
                    // cout << "TRA_ctrl_header generated" << endl;
                    return new TRA_ctrl_header; 
                }
            public:
                virtual string type() { return "TRA_ctrl_header";}
                ~TRA_ctrl_header_generator(){}
        
        };
};
TRA_ctrl_header::TRA_ctrl_header_generator TRA_ctrl_header::TRA_ctrl_header_generator::sample;


class SDN_ctrl_header : public header{
        SDN_ctrl_header(SDN_ctrl_header&){} // cannot be called by users
        
    protected:
        SDN_ctrl_header(){} // this constructor cannot be directly called by users

    public:
        ~SDN_ctrl_header(){}
        string type() { return "SDN_ctrl_header"; }

        class SDN_ctrl_header_generator;
        friend class SDN_ctrl_header_generator;
        // SDN_ctrl_header_generator is derived from header_generator to generate a header
        class SDN_ctrl_header_generator : public header_generator{
                static SDN_ctrl_header_generator sample;
                // this constructor is only for sample to register this header type
                SDN_ctrl_header_generator() { /*cout << "SDN_ctrl_header registered" << endl;*/ register_header_type(&sample); }
            protected:
                virtual header * generate(){
                    // cout << "SDN_ctrl_header generated" << endl;
                    return new SDN_ctrl_header; 
                }
            public:
                virtual string type() { return "SDN_ctrl_header";}
                ~SDN_ctrl_header_generator(){}
        
        };
};
SDN_ctrl_header::SDN_ctrl_header_generator SDN_ctrl_header::SDN_ctrl_header_generator::sample;



class payload {
        payload(payload&){} // this constructor cannot be directly called by users
        
        string msg;
        
    protected:
        payload(){}
    public:
        virtual ~payload(){}
        virtual string type() = 0;
        
        SET(setMsg,string,msg,_msg);
        GET(getMsg,string,msg);
        
        class payload_generator {
                // lock the copy constructor
                payload_generator(payload_generator &){}
                // store all possible types of header
                static map<string,payload_generator*> prototypes;
            protected:
                // allow derived class to use it
                payload_generator() {}
                // after you create a new payload type, please register the factory of this payload type by this function
                void register_payload_type(payload_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your payload
                virtual payload* generate() = 0;
            public:
                // you have to implement your own type() to return your header type
        	    virtual string type() = 0;
        	    // this function is used to generate any type of header derived
        	    static payload * generate (string type) {
            		if(prototypes.find(type) != prototypes.end()){ // if this type derived exists 
            			return prototypes[type]->generate(); // generate it!!
            		}
            		std::cerr << "no such payload type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered payload types: " << endl;
            	    for (map<string,payload::payload_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~payload_generator(){};
        };
};
map<string,payload::payload_generator*> payload::payload_generator::prototypes;


class TRA_data_payload : public payload {
        TRA_data_payload(TRA_data_payload&){}

    protected:
        TRA_data_payload(){} // this constructor cannot be directly called by users
    public:
        ~TRA_data_payload(){}
        
        string type() { return "TRA_data_payload"; }
        
        class TRA_data_payload_generator;
        friend class TRA_data_payload_generator;
        // TRA_data_payload is derived from payload_generator to generate a payload
        class TRA_data_payload_generator : public payload_generator{
                static TRA_data_payload_generator sample;
                // this constructor is only for sample to register this payload type
                TRA_data_payload_generator() { /*cout << "TRA_data_payload registered" << endl;*/ register_payload_type(&sample); }
            protected:
                virtual payload * generate(){ 
                    // cout << "TRA_data_payload generated" << endl;
                    return new TRA_data_payload; 
                }
            public:
                virtual string type() { return "TRA_data_payload";}
                ~TRA_data_payload_generator(){}
        };
};
TRA_data_payload::TRA_data_payload_generator TRA_data_payload::TRA_data_payload_generator::sample;

class TRA_ctrl_payload : public payload {
        TRA_ctrl_payload(TRA_ctrl_payload & s): counter (s.counter) {}
        
        unsigned int counter ;
        
    protected:
        TRA_ctrl_payload(): counter (0) {} // this constructor cannot be directly called by users
    public:
        ~TRA_ctrl_payload(){}
        
        void increase() { counter ++; } // used to increase the counter
        GET(getCounter,unsigned int,counter); // used to get the value of counter
        
        string type() { return "TRA_ctrl_payload"; }
        
        
        class TRA_ctrl_payload_generator;
        friend class TRA_ctrl_payload_generator;
        // TRA_data_payload is derived from payload_generator to generate a payload
        class TRA_ctrl_payload_generator : public payload_generator{
                static TRA_ctrl_payload_generator sample;
                // this constructor is only for sample to register this payload type
                TRA_ctrl_payload_generator() { /*cout << "TRA_ctrl_payload registered" << endl;*/ register_payload_type(&sample); }
            protected:
                virtual payload * generate(){ 
                    // cout << "TRA_ctrl_payload generated" << endl;
                    return new TRA_ctrl_payload; 
                }
            public:
                virtual string type() { return "TRA_ctrl_payload";}
                ~TRA_ctrl_payload_generator(){}
        };
};
TRA_ctrl_payload::TRA_ctrl_payload_generator TRA_ctrl_payload::TRA_ctrl_payload_generator::sample;

class SDN_ctrl_payload : public payload {
        SDN_ctrl_payload(SDN_ctrl_payload&){}
        
        unsigned int matID; // match: target
        unsigned int actID; // action: the next hop
        double per; // percentage

    protected:
        SDN_ctrl_payload(){} // this constructor cannot be directly called by users
    public:
        ~SDN_ctrl_payload(){}

        string type() { return "SDN_ctrl_payload"; }
        
        SET(setMatID,unsigned int,matID,_matID);
        GET(getMatID,unsigned int,matID);
        SET(setActID,unsigned int,actID,_actID);
        GET(getActID,unsigned int,actID);
        SET(setPer,double,per,_per);
        GET(getPer,double,per);
        
        
        class SDN_ctrl_payload_generator;
        friend class SDN_ctrl_payload_generator;
        // SDN_data_payload is derived from payload_generator to generate a payload
        class SDN_ctrl_payload_generator : public payload_generator{
                static SDN_ctrl_payload_generator sample;
                // this constructor is only for sample to register this payload type
                SDN_ctrl_payload_generator() { /*cout << "SDN_ctrl_payload registered" << endl;*/ register_payload_type(&sample); }
            protected:
                virtual payload * generate(){ 
                    // cout << "SDN_ctrl_payload generated" << endl;
                    return new SDN_ctrl_payload; 
                }
            public:
                virtual string type() { return "SDN_ctrl_payload";}
                ~SDN_ctrl_payload_generator(){}
        };
};
SDN_ctrl_payload::SDN_ctrl_payload_generator SDN_ctrl_payload::SDN_ctrl_payload_generator::sample;


class packet{
        // a packet usually contains a header and a payload
        header *hdr;
        payload *pld;
        unsigned int p_id;
        static unsigned int last_packet_id ; 
        
        packet(packet &) {}
        static int live_packet_num ;
    protected:
        // these constructors cannot be directly called by users
        packet(): hdr(nullptr), pld(nullptr) { p_id=last_packet_id++; live_packet_num ++; }
        packet(string _hdr, string _pld, bool rep = false, unsigned int rep_id = 0) {
            if (! rep ) // a duplicated packet does not have a new packet id
                p_id = last_packet_id ++;
            else
                p_id = rep_id;
            hdr = header::header_generator::generate(_hdr); 
            pld = payload::payload_generator::generate(_pld); 
            live_packet_num ++;
        }
    public:
        virtual ~packet(){ 
            // cout << "packet destructor begin" << endl;
            if (hdr != nullptr) 
                delete hdr; 
            if (pld != nullptr) 
                delete pld; 
            live_packet_num --;
            // cout << "packet destructor end" << endl;
        }
        
        SET(setHeader,header*,hdr,_hdr);
        GET(getHeader,header*,hdr);
        SET(setPayload,payload*,pld,_pld);
        GET(getPayload,payload*,pld);
        GET(getPacketID,unsigned int,p_id);
        
        static void discard ( packet* &p ) {
            // cout << "checking" << endl;
            if (p != nullptr) {
                // cout << "discarding" << endl;
                // cout << p->type() << endl;
                delete p;
                // cout << "discarded" << endl;
            }
            p = nullptr;
            // cout << "checked" << endl;
        }
        virtual string type () = 0;
        virtual string addition_information() { return ""; }
        
        static int getLivePacketNum () { return live_packet_num; }
        
        class packet_generator;
        friend class packet_generator;
        class packet_generator {
                // lock the copy constructor
                packet_generator(packet_generator &){}
                // store all possible types of packet
                static map<string,packet_generator*> prototypes;
            protected:
                // allow derived class to use it
                packet_generator() {}
                // after you create a new packet type, please register the factory of this payload type by this function
                void register_packet_type(packet_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your payload
                virtual packet* generate ( packet *p = nullptr) = 0;
            public:
                // you have to implement your own type() to return your packet type
        	    virtual string type() = 0;
        	    // this function is used to generate any type of packet derived
        	    static packet * generate (string type) {
            		if(prototypes.find(type) != prototypes.end()){ // if this type derived exists 
            			return prototypes[type]->generate(); // generate it!!
            		}
            		std::cerr << "no such packet type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static packet * replicate (packet *p) {
            	    if(prototypes.find(p->type()) != prototypes.end()){ // if this type derived exists 
            			return prototypes[p->type()]->generate(p); // generate it!!
            		}
            		std::cerr << "no such packet type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered packet types: " << endl;
            	    for (map<string,packet::packet_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~packet_generator(){};
        };
};
map<string,packet::packet_generator*> packet::packet_generator::prototypes;
unsigned int packet::last_packet_id = 0 ;
int packet::live_packet_num = 0;


// this packet is used to tell the destination the msg
class TRA_data_packet: public packet {
        TRA_data_packet(TRA_data_packet &) {}
        
    protected:
        TRA_data_packet(){} // this constructor cannot be directly called by users
        TRA_data_packet(packet*p): packet(p->getHeader()->type(), p->getPayload()->type(), true, p->getPacketID()) {
            *(dynamic_cast<TRA_data_header*>(this->getHeader())) = *(dynamic_cast<TRA_data_header*> (p->getHeader()));
            *(dynamic_cast<TRA_data_payload*>(this->getPayload())) = *(dynamic_cast<TRA_data_payload*> (p->getPayload()));
            //DFS_path = (dynamic_cast<TRA_data_header*>(p))->DFS_path;
            //isVisited = (dynamic_cast<TRA_data_header*>(p))->isVisited;
        } // for duplicate
        TRA_data_packet(string _h, string _p): packet(_h,_p) {}
        
    public:
        virtual ~TRA_data_packet(){}
        string type() { return "TRA_data_packet"; }
        
        class TRA_data_packet_generator;
        friend class TRA_data_packet_generator;
        // TRA_data_packet is derived from packet_generator to generate a pub packet
        class TRA_data_packet_generator : public packet_generator{
                static TRA_data_packet_generator sample;
                // this constructor is only for sample to register this packet type
                TRA_data_packet_generator() { /*cout << "TRA_data_packet registered" << endl;*/ register_packet_type(&sample); }
            protected:
                virtual packet *generate (packet *p = nullptr){
                    // cout << "TRA_data_packet generated" << endl;
                    if ( nullptr == p )
                        return new TRA_data_packet("TRA_data_header","TRA_data_payload"); 
                    else
                        return new TRA_data_packet(p); // duplicate
                }
            public:
                virtual string type() { return "TRA_data_packet";}
                ~TRA_data_packet_generator(){}
        };
};
TRA_data_packet::TRA_data_packet_generator TRA_data_packet::TRA_data_packet_generator::sample;

// this packet type is used to tell the node what should be installed in the routing table
class TRA_ctrl_packet: public packet {
        TRA_ctrl_packet(TRA_ctrl_packet &) {}
        
    protected:
        TRA_ctrl_packet(){} // this constructor cannot be directly called by users
        TRA_ctrl_packet(packet*p): packet(p->getHeader()->type(), p->getPayload()->type(), true, p->getPacketID()) {
            *(dynamic_cast<TRA_ctrl_header*>(this->getHeader())) = *(dynamic_cast<TRA_ctrl_header*> (p->getHeader()));
            *(dynamic_cast<TRA_ctrl_payload*>(this->getPayload())) = *(dynamic_cast<TRA_ctrl_payload*> (p->getPayload()));
            //DFS_path = (dynamic_cast<TRA_ctrl_header*>(p))->DFS_path;
            //isVisited = (dynamic_cast<TRA_ctrl_header*>(p))->isVisited;
        } // for duplicate
        TRA_ctrl_packet(string _h, string _p): packet(_h,_p) {}
        
    public:
        virtual ~TRA_ctrl_packet(){}
        string type() { return "TRA_ctrl_packet"; }
        virtual string addition_information() {
            unsigned int counter = (dynamic_cast<TRA_ctrl_payload*>(this->getPayload()))->getCounter();
            // cout << counter << endl;
            return " counter " + to_string(counter);
        }
        
        class TRA_ctrl_packet_generator;
        friend class TRA_ctrl_packet_generator;
        // TRA_ctrl_packet is derived from packet_generator to generate a pub packet
        class TRA_ctrl_packet_generator : public packet_generator{
                static TRA_ctrl_packet_generator sample;
                // this constructor is only for sample to register this packet type
                TRA_ctrl_packet_generator() { /*cout << "TRA_ctrl_packet registered" << endl;*/ register_packet_type(&sample); }
            protected:
                virtual packet *generate (packet *p = nullptr){
                    // cout << "TRA_ctrl_packet generated" << endl;
                    if ( nullptr == p )
                        return new TRA_ctrl_packet("TRA_ctrl_header","TRA_ctrl_payload"); 
                    else
                        return new TRA_ctrl_packet(p); // duplicate
                }
            public:
                virtual string type() { return "TRA_ctrl_packet";}
                ~TRA_ctrl_packet_generator(){}
        };
};
TRA_ctrl_packet::TRA_ctrl_packet_generator TRA_ctrl_packet::TRA_ctrl_packet_generator::sample;


class SDN_ctrl_packet: public packet {
        SDN_ctrl_packet(SDN_ctrl_packet &) {}
        
    protected:
        SDN_ctrl_packet(){} // this constructor cannot be directly called by users
        SDN_ctrl_packet(packet*p): packet(p->getHeader()->type(), p->getPayload()->type(), true, p->getPacketID()) {
            *(dynamic_cast<SDN_ctrl_header*>(this->getHeader())) = *(dynamic_cast<SDN_ctrl_header*> (p->getHeader()));
            *(dynamic_cast<SDN_ctrl_payload*>(this->getPayload())) = *(dynamic_cast<SDN_ctrl_payload*> (p->getPayload()));
            //DFS_path = (dynamic_cast<SDN_ctrl_header*>(p))->DFS_path;
            //isVisited = (dynamic_cast<SDN_ctrl_header*>(p))->isVisited;
        } // for duplicate
        SDN_ctrl_packet(string _h, string _p): packet(_h,_p) {}
        
    public:
        virtual ~SDN_ctrl_packet(){}
        string type() { return "SDN_ctrl_packet"; }
        
        class SDN_ctrl_packet_generator;
        friend class SDN_ctrl_packet_generator;
        // SDN_ctrl_packet is derived from packet_generator to generate a pub packet
        class SDN_ctrl_packet_generator : public packet_generator{
                static SDN_ctrl_packet_generator sample;
                // this constructor is only for sample to register this packet type
                SDN_ctrl_packet_generator() { /*cout << "SDN_ctrl_packet registered" << endl;*/ register_packet_type(&sample); }
            protected:
                virtual packet *generate (packet *p = nullptr){
                    // cout << "SDN_ctrl_packet generated" << endl;
                    if ( nullptr == p )
                        return new SDN_ctrl_packet("SDN_ctrl_header","SDN_ctrl_payload"); 
                    else
                        return new SDN_ctrl_packet(p); // duplicate
                }
            public:
                virtual string type() { return "SDN_ctrl_packet";}
                ~SDN_ctrl_packet_generator(){}
        };
};
SDN_ctrl_packet::SDN_ctrl_packet_generator SDN_ctrl_packet::SDN_ctrl_packet_generator::sample;




class node {
        // all nodes created in the program
        static map<unsigned int, node*> id_node_table;
        
        unsigned int id;
        map<unsigned int,bool> phy_neighbors;
        
    protected:
        node(node&){} // this constructor should not be used
        node(){} // this constructor should not be used
        node(unsigned int _id): id(_id) { id_node_table[_id] = this; }
    public:
        virtual ~node() { // erase the node
            id_node_table.erase (id) ;
        }
        virtual string type() = 0; // please define it in your derived node class
        
        void add_phy_neighbor (unsigned int _id, string link_type = "simple_link"); // we only add a directed link from id to _id
        void del_phy_neighbor (unsigned int _id); // we only delete a directed link from id to _id
        
        // you can use the function to get the node's neigbhors at this time
        // but in the project 3, you are not allowed to use this function 
        const map<unsigned int,bool> & getPhyNeighbors () { 
            return phy_neighbors;
        }
        
        
        void recv (packet *p) {
            packet *tp = p;
            recv_handler(tp); 
            packet::discard(p); 
        } // the packet will be directly deleted after the handler
        void send (packet *p);
        
        // receive the packet and do something; this is a pure virtual function
        virtual void recv_handler(packet *p) = 0;
        void send_handler(packet *P);
        
        static node * id_to_node (unsigned int _id) { return ((id_node_table.find(_id)!=id_node_table.end()) ? id_node_table[_id]: nullptr) ; }
        GET(getNodeID,unsigned int,id);
        
        static void del_node (unsigned int _id) {
            if (id_node_table.find(_id) != id_node_table.end())
                id_node_table.erase(_id);
        }
        static unsigned int getNodeNum () { return id_node_table.size(); }

        class node_generator {
                // lock the copy constructor
                node_generator(node_generator &){}
                // store all possible types of node
                static map<string,node_generator*> prototypes;
            protected:
                // allow derived class to use it
                node_generator() {}
                // after you create a new node type, please register the factory of this node type by this function
                void register_node_type(node_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your node
                virtual node* generate(unsigned int _id) = 0;
            public:
                // you have to implement your own type() to return your node type
        	    virtual string type() = 0;
        	    // this function is used to generate any type of node derived
        	    static node * generate (string type, unsigned int _id) {
        	        if(id_node_table.find(_id)!=id_node_table.end()){
        	            std::cerr << "duplicate node id" << std::endl; // node id is duplicated
        	            return nullptr;
        	        }
        	        else if ( BROCAST_ID == _id ) {
        	            cerr << "BROCAST_ID cannot be used" << endl;
        	            return nullptr;
        	        }
            		else if(prototypes.find(type) != prototypes.end()){ // if this type derived exists 
            		    node * created_node = prototypes[type]->generate(_id);
            			return created_node; // generate it!!
            		}
            		std::cerr << "no such node type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered node types: " << endl;
            	    for (map<string,node::node_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~node_generator(){};
        };
};
map<string,node::node_generator*> node::node_generator::prototypes;
map<unsigned int,node*> node::id_node_table;

class TRA_switch: public node {
        // map<unsigned int,bool> one_hop_neighbors; // you can use this variable to record the node's 1-hop neighbors 
        
        map<unsigned int, int> turn; // record the times of receiving packet in this node
        map<unsigned int, int> parent_counter;

    protected:
        TRA_switch() {} // it should not be used
        TRA_switch(TRA_switch&) {} // it should not be used
        TRA_switch(unsigned int _id): node(_id) {} // this constructor cannot be directly called by users
    
    public:
        ~TRA_switch(){}
        string type() { return "TRA_switch"; }
        
        // please define recv_handler function to deal with the incoming packet
        virtual void recv_handler (packet *p);
        
        // void add_one_hop_neighbor (unsigned int n_id) { one_hop_neighbors[n_id] = true; }
        // unsigned int get_one_hop_neighbor_num () { return one_hop_neighbors.size(); }
        //------------------ add -----------------------------
        map<unsigned int, unsigned int> routing_table;
        void Create_map(unsigned int dst, unsigned int next_hop) {
            routing_table[dst] = next_hop;
        }
        const unsigned int Get_Id_From_Table(const unsigned int src_id) {
            return routing_table[src_id];
        }
        bool Is_exist_turn(const unsigned int src_id) {
            if(turn.find(src_id) != turn.end()) {
                return true;
            } else {
                return false;
            }
        }
        void Set_turn(const unsigned int src_id) {
            turn[src_id] = 1;
        }
        const int Get_turn(const unsigned int src_id) {
            return turn[src_id];
        }
        void Set_parent_counter(const unsigned int src_id, const int pre_counter) {
            parent_counter[src_id] = pre_counter;
        }
        const int Get_parent_counter(const unsigned int src_id) {
            return parent_counter[src_id];
        }
        //-----------------------------------------------
        
        class TRA_switch_generator;
        friend class TRA_switch_generator;
        // TRA_switch is derived from node_generator to generate a node
        class TRA_switch_generator : public node_generator{
                static TRA_switch_generator sample;
                // this constructor is only for sample to register this node type
                TRA_switch_generator() { /*cout << "TRA_switch registered" << endl;*/ register_node_type(&sample); }
            protected:
                virtual node * generate(unsigned int _id){ /*cout << "TRA_switch generated" << endl;*/ return new TRA_switch(_id); }
            public:
                virtual string type() { return "TRA_switch";}
                ~TRA_switch_generator(){}
        };
};
TRA_switch::TRA_switch_generator TRA_switch::TRA_switch_generator::sample;

//---------------------------------------------------
class SDN_switch: public node {
        // map<unsigned int,bool> one_hop_neighbors; // you can use this variable to record the node's 1-hop neighbors 
        
        map<unsigned int, vector< pair<unsigned int, double> > > sdn_table; // dst, < nexthop id , ratio >
    protected:
        SDN_switch() {} // it should not be used
        SDN_switch(SDN_switch&) {} // it should not be used
        SDN_switch(unsigned int _id): node(_id) {} // this constructor cannot be directly called by users
    
    public:
        ~SDN_switch(){}
        string type() { return "SDN_switch"; }
        
        // please define recv_handler function to deal with the incoming packet
        virtual void recv_handler (packet *p);
        
        // void add_one_hop_neighbor (unsigned int n_id) { one_hop_neighbors[n_id] = true; }
        // unsigned int get_one_hop_neighbor_num () { return one_hop_neighbors.size(); }
        //------------------add--------------------
        void setSdnTable(const unsigned int dst_id, const unsigned int next_hop, const double ratio) {
          sdn_table[dst_id].push_back(make_pair(next_hop, ratio));
        }
        const unsigned int getNextHop(const unsigned int dst_id, int index) {
          return sdn_table[dst_id][index].first;
        }
        const unsigned int getRatio(const unsigned int dst_id, int index) {
          return sdn_table[dst_id][index].second;
        }
        const unsigned int getSize(const unsigned int dst_id) {
          return sdn_table[dst_id].size();
        }
        //----------------------------------------- class SDN_switch_generator;
        friend class SDN_switch_generator;
        // SDN_switch is derived from node_generator to generate a node
        class SDN_switch_generator : public node_generator{
                static SDN_switch_generator sample;
                // this constructor is only for sample to register this node type
                SDN_switch_generator() { /*cout << "SDN_switch registered" << endl;*/ register_node_type(&sample); }
            protected:
                virtual node * generate(unsigned int _id){ /*cout << "SDN_switch generated" << endl;*/ return new SDN_switch(_id); }
            public:
                virtual string type() { return "SDN_switch";}
                ~SDN_switch_generator(){}
        };
};
SDN_switch::SDN_switch_generator SDN_switch::SDN_switch_generator::sample;

class SDN_controller: public node {
        // map<unsigned int,bool> one_hop_neighbors; // you can use this variable to record the node's 1-hop neighbors 
        
        // bool hi; // this is used for example; you can remove it when doing hw2

    protected:
        SDN_controller() {} // it should not be used
        SDN_controller(SDN_controller&) {} // it should not be used
        SDN_controller(unsigned int _id): node(_id) {} // this constructor cannot be directly called by users
    
    public:
        ~SDN_controller(){}
        string type() { return "SDN_controller"; }
        
        // please define recv_handler function to deal with the incoming packet
        virtual void recv_handler (packet *p);
        
        // void add_one_hop_neighbor (unsigned int n_id) { one_hop_neighbors[n_id] = true; }
        // unsigned int get_one_hop_neighbor_num () { return one_hop_neighbors.size(); }
        
        class SDN_controller_generator;
        friend class SDN_controller_generator;
        // SDN_controller is derived from node_generator to generate a node
        class SDN_controller_generator : public node_generator{
                static SDN_controller_generator sample;
                // this constructor is only for sample to register this node type
                SDN_controller_generator() { /*cout << "SDN_controller registered" << endl;*/ register_node_type(&sample); }
            protected:
                virtual node * generate(unsigned int _id){ /*cout << "SDN_controller generated" << endl;*/ return new SDN_controller(_id); }
            public:
                virtual string type() { return "SDN_controller";}
                ~SDN_controller_generator(){}
        };
};
SDN_controller::SDN_controller_generator SDN_controller::SDN_controller_generator::sample;
//---------------------------------------------------

class mycomp {
    bool reverse;
    
    public:
        mycomp(const bool& revparam = false) { reverse=revparam ; }
        bool operator() (const event* lhs, const event* rhs) const;
};

class event {
        event(event*&){} // this constructor cannot be directly called by users
        static priority_queue < event*, vector < event* >, mycomp > events;
        static unsigned int cur_time; // timer
        static unsigned int end_time;
        
        // get the next event
        static event * get_next_event() ;
        static void add_event (event *e) { events.push(e); }
        static hash<string> event_seq;
        
    protected:
        unsigned int trigger_time;
        
        event(){} // it should not be used
        event(unsigned int _trigger_time): trigger_time(_trigger_time) {}

    public:
        virtual void trigger()=0;
        virtual ~event(){}

        virtual unsigned int event_priority() const = 0;
        unsigned int get_hash_value(string string_for_hash) const {
            unsigned int priority = event_seq (string_for_hash);
            return priority;
        }
        
        static void flush_events (); // only for debug
        
        GET(getTriggerTime,unsigned int,trigger_time);
        
        static void start_simulate( unsigned int _end_time ); // the function is used to start the simulation
        
        static unsigned int getCurTime() { return cur_time ; }
        static void getCurTime(unsigned int _cur_time) { cur_time = _cur_time; } 
        // static unsigned int getEndTime() { return end_time ; }
        // static void getEndTime(unsigned int _end_time) { end_time = _end_time; }
        
        virtual void print () const = 0; // the function is used to print the event information

        class event_generator{
                // lock the copy constructor
                event_generator(event_generator &){}
                // store all possible types of event
                static map<string,event_generator*> prototypes;
            protected:
                // allow derived class to use it
                event_generator() {}
                // after you create a new event type, please register the factory of this event type by this function
                void register_event_type(event_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your event
                virtual event* generate(unsigned int _trigger_time, void * data) = 0;
            public:
                // you have to implement your own type() to return your event type
        	    virtual string type() = 0;
        	    // this function is used to generate any type of event derived
        	    static event * generate (string type, unsigned int _trigger_time, void * data) {
            		if(prototypes.find(type) != prototypes.end()){ // if this type derived exists
            		    event * e = prototypes[type]->generate(_trigger_time, data);
            		    add_event(e);
            		    return e; // generate it!!
            		}
            		std::cerr << "no such event type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered event types: " << endl;
            	    for (map<string,event::event_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~event_generator(){}
        };
};
map<string,event::event_generator*> event::event_generator::prototypes;
priority_queue < event*, vector< event* >, mycomp > event::events;
hash<string> event::event_seq;

unsigned int event::cur_time = 0;
unsigned int event::end_time = 0;

void event::flush_events()
{ 
    cout << "**flush begin" << endl;
    while ( ! events.empty() ) {
        cout << setw(11) << events.top()->trigger_time << ": " << setw(11) << events.top()->event_priority() << endl;
        delete events.top();
        events.pop();
    }
    cout << "**flush end" << endl;
}
event * event::get_next_event() {
    if(events.empty()) 
        return nullptr; 
    event * e = events.top();
    events.pop(); 
    // cout << events.size() << " events remains" << endl;
    return e; 
}
void event::start_simulate(unsigned int _end_time) {
    if (_end_time<0) {
        cerr << "you should give a possitive value of _end_time" << endl;
        return;
    }
    end_time = _end_time;
    event *e; 
    e = event::get_next_event ();
    while ( e != nullptr && e->trigger_time <= end_time ) {
        if ( cur_time <= e->trigger_time )
            cur_time = e->trigger_time;
        else {
            cerr << "cur_time = " << cur_time << ", event trigger_time = " << e->trigger_time << endl;
            break;
        }

        // cout << "event trigger_time = " << e->trigger_time << endl;
        e->print(); // for log
        // cout << " event begin" << endl;
        e->trigger();
        // cout << " event end" << endl;
        delete e;
        e = event::get_next_event ();
    }
    // cout << "no more event" << endl;
}

bool mycomp::operator() (const event* lhs, const event* rhs) const {
    // cout << lhs->getTriggerTime() << ", " << rhs->getTriggerTime() << endl;
    // cout << lhs->type() << ", " << rhs->type() << endl;
    unsigned int lhs_pri = lhs->event_priority();
    unsigned int rhs_pri = rhs->event_priority();
    // cout << "lhs hash = " << lhs_pri << endl;
    // cout << "rhs hash = " << rhs_pri << endl;
    
    if (reverse) 
        return ((lhs->getTriggerTime()) == (rhs->getTriggerTime())) ? (lhs_pri < rhs_pri): ((lhs->getTriggerTime()) < (rhs->getTriggerTime()));
    else 
        return ((lhs->getTriggerTime()) == (rhs->getTriggerTime())) ? (lhs_pri > rhs_pri): ((lhs->getTriggerTime()) > (rhs->getTriggerTime()));
}

class recv_event: public event {
    public:
        class recv_data; // forward declaration
            
    private:
        recv_event(recv_event&) {} // this constructor cannot be used
        recv_event() {} // we don't allow users to new a recv_event by themselv
        unsigned int senderID; // the sender
        unsigned int receiverID; // the receiver; the packet will be given to the receiver
        packet *pkt; // the packet
        
    protected:
        // this constructor cannot be directly called by users; only by generator
        recv_event(unsigned int _trigger_time, void *data): event(_trigger_time), senderID(BROCAST_ID), receiverID(BROCAST_ID), pkt(nullptr){
            recv_data * data_ptr = (recv_data*) data;
            senderID = data_ptr->s_id;
            receiverID = data_ptr->r_id; // the packet will be given to the receiver
            pkt = data_ptr->_pkt;
        } 
        
    public:
        virtual ~recv_event(){}
        // recv_event will trigger the recv function
        virtual void trigger();
        
        unsigned int event_priority() const;
        
        class recv_event_generator;
        friend class recv_event_generator;
        // recv_event is derived from event_generator to generate a event
        class recv_event_generator : public event_generator{
                static recv_event_generator sample;
                // this constructor is only for sample to register this event type
                recv_event_generator() { /*cout << "recv_event registered" << endl;*/ register_event_type(&sample); }
            protected:
                virtual event * generate(unsigned int _trigger_time, void *data){ 
                    // cout << "recv_event generated" << endl; 
                    return new recv_event(_trigger_time, data); 
                }
                
            public:
                virtual string type() { return "recv_event";}
                ~recv_event_generator(){}
        };
        // this class is used to initialize the recv_event
        class recv_data{
            public:
                unsigned int s_id;
                unsigned int r_id;
                packet *_pkt;
        };
        
        void print () const;
};
recv_event::recv_event_generator recv_event::recv_event_generator::sample;

void recv_event::trigger() {
    if (pkt == nullptr) {
        cerr << "recv_event error: no pkt!" << endl; 
        return ; 
    }
    else if (node::id_to_node(receiverID) == nullptr){
        cerr << "recv_event error: no node " << receiverID << "!" << endl;
        delete pkt; return ;
    }
    node::id_to_node(receiverID)->recv(pkt); 
}
unsigned int recv_event::event_priority() const {
    string string_for_hash;
    string_for_hash = to_string(getTriggerTime()) + to_string(senderID) + to_string (receiverID) + to_string (pkt->getPacketID());
    return get_hash_value(string_for_hash);
}
// the recv_event::print() function is used for log file
void recv_event::print () const {
    cout << "time "          << setw(11) << event::getCurTime() 
         << "   recID"       << setw(11) << receiverID 
         << "   pktID"       << setw(11) << pkt->getPacketID()
         << "   srcID"       << setw(11) << pkt->getHeader()->getSrcID() 
         << "   dstID"       << setw(11) << pkt->getHeader()->getDstID() 
         << "   preID"       << setw(11) << pkt->getHeader()->getPreID()
         << "   nexID"       << setw(11) << pkt->getHeader()->getNexID()
         << "   "            << pkt->type()
         << pkt->addition_information();
//  if ( pkt->type() == "TRA_ctrl_packet" ) cout << "   " << ((TRA_ctrl_payload*)pkt->getPayload())->getCounter();
         cout << endl;
    // cout << pkt->type()
    //      << "   time "       << setw(11) << event::getCurTime() 
    //      << "   recID "      << setw(11) << receiverID 
    //      << "   pktID"       << setw(11) << pkt->getPacketID()
    //      << "   srcID "      << setw(11) << pkt->getHeader()->getSrcID() 
    //      << "   dstID"       << setw(11) << pkt->getHeader()->getDstID() 
    //      << "   preID"       << setw(11) << pkt->getHeader()->getPreID()
    //      << "   nexID"       << setw(11) << pkt->getHeader()->getNexID()
    //      << endl;
}

class send_event: public event {
    public:
        class send_data; // forward declaration
            
    private:
        send_event (send_event &){}
        send_event (){} // we don't allow users to new a recv_event by themselves
        // this constructor cannot be directly called by users; only by generator
        unsigned int senderID; // the sender
        unsigned int receiverID; // the receiver 
        packet *pkt; // the packet
    
    protected:
        send_event (unsigned int _trigger_time, void *data): event(_trigger_time), senderID(BROCAST_ID), receiverID(BROCAST_ID), pkt(nullptr){
            send_data * data_ptr = (send_data*) data;
            senderID = data_ptr->s_id;
            receiverID = data_ptr->r_id;
            pkt = data_ptr->_pkt;
        } 
        
    public:
        virtual ~send_event(){}
        // send_event will trigger the send function
        virtual void trigger();
        
        unsigned int event_priority() const;
        
        class send_event_generator;
        friend class send_event_generator;
        // send_event is derived from event_generator to generate a event
        class send_event_generator : public event_generator{
                static send_event_generator sample;
                // this constructor is only for sample to register this event type
                send_event_generator() { /*cout << "send_event registered" << endl;*/ register_event_type(&sample); }
            protected:
                virtual event * generate(unsigned int _trigger_time, void *data){ 
                    // cout << "send_event generated" << endl; 
                    return new send_event(_trigger_time, data); 
                }
            
            public:
                virtual string type() { return "send_event";}
                ~send_event_generator(){}
        };
        // this class is used to initialize the send_event
        class send_data{
            public:
                unsigned int s_id;
                unsigned int r_id;
                packet *_pkt;
                unsigned int t;
        };
        
        void print () const;
};
send_event::send_event_generator send_event::send_event_generator::sample;

void send_event::trigger() {
    if (pkt == nullptr) {
        cerr << "send_event error: no pkt!" << endl; 
        return ; 
    }
    else if (node::id_to_node(senderID) == nullptr){
        cerr << "send_event error: no node " << senderID << "!" << endl;
        delete pkt; return ;
    }
    node::id_to_node(senderID)->send(pkt);
}
unsigned int send_event::event_priority() const {
    string string_for_hash;
    string_for_hash = to_string(getTriggerTime()) + to_string(senderID) + to_string (receiverID) + to_string (pkt->getPacketID());
    return get_hash_value(string_for_hash);
}
// the send_event::print() function is used for log file
void send_event::print () const {
    cout << "time "          << setw(11) << event::getCurTime() 
         << "   senID"       << setw(11) << senderID 
         << "   pktID"       << setw(11) << pkt->getPacketID()
         << "   srcID"       << setw(11) << pkt->getHeader()->getSrcID() 
         << "   dstID"       << setw(11) << pkt->getHeader()->getDstID() 
         << "   preID"       << setw(11) << pkt->getHeader()->getPreID()
         << "   nexID"       << setw(11) << pkt->getHeader()->getNexID()
         << "   "            << pkt->type()
         << pkt->addition_information()
         //<< "   msg"         << setw(11) << dynamic_cast<TRA_data_payload*>(pkt->getPayload())->getMsg()
         << endl;
    // cout << pkt->type()
    //      << "   time "       << setw(11) << event::getCurTime() 
    //      << "   senID "      << setw(11) << senderID
    //      << "   pktID"       << setw(11) << pkt->getPacketID()
    //      << "   srcID "      << setw(11) << pkt->getHeader()->getSrcID() 
    //      << "   dstID"       << setw(11) << pkt->getHeader()->getDstID() 
    //      << "   preID"       << setw(11) << pkt->getHeader()->getPreID()
    //      << "   nexID"       << setw(11) << pkt->getHeader()->getNexID()
    //      << endl;
}

////////////////////////////////////////////////////////////////////////////////

class TRA_data_pkt_gen_event: public event {
    public:
        class gen_data; // forward declaration
            
    private:
        TRA_data_pkt_gen_event (TRA_data_pkt_gen_event &){}
        TRA_data_pkt_gen_event (){} // we don't allow users to new a recv_event by themselves
        // this constructor cannot be directly called by users; only by generator
        unsigned int src; // the src
        unsigned int dst; // the dst 
        // packet *pkt; // the packet
        string msg;
    
    protected:
        TRA_data_pkt_gen_event (unsigned int _trigger_time, void *data): event(_trigger_time), src(BROCAST_ID), dst(BROCAST_ID){
            pkt_gen_data * data_ptr = (pkt_gen_data*) data;
            src = data_ptr->src_id;
            dst = data_ptr->dst_id;
            // pkt = data_ptr->_pkt;
            msg = data_ptr->msg;
        } 
        
    public:
        virtual ~TRA_data_pkt_gen_event(){}
        // TRA_data_pkt_gen_event will trigger the packet gen function
        virtual void trigger();
        
        unsigned int event_priority() const;
        
        class TRA_data_pkt_gen_event_generator;
        friend class TRA_data_pkt_gen_event_generator;
        // TRA_data_pkt_gen_event_generator is derived from event_generator to generate an event
        class TRA_data_pkt_gen_event_generator : public event_generator{
                static TRA_data_pkt_gen_event_generator sample;
                // this constructor is only for sample to register this event type
                TRA_data_pkt_gen_event_generator() { /*cout << "send_event registered" << endl;*/ register_event_type(&sample); }
            protected:
                virtual event * generate(unsigned int _trigger_time, void *data){ 
                    // cout << "send_event generated" << endl; 
                    return new TRA_data_pkt_gen_event(_trigger_time, data); 
                }
            
            public:
                virtual string type() { return "TRA_data_pkt_gen_event";}
                ~TRA_data_pkt_gen_event_generator(){}
        };
        // this class is used to initialize the TRA_data_pkt_gen_event
        class pkt_gen_data{
            public:
                unsigned int src_id;
                unsigned int dst_id;
                string msg;
                // packet *_pkt;
        };
        
        void print () const;
};
TRA_data_pkt_gen_event::TRA_data_pkt_gen_event_generator TRA_data_pkt_gen_event::TRA_data_pkt_gen_event_generator::sample;

void TRA_data_pkt_gen_event::trigger() {
    if (node::id_to_node(src) == nullptr){
        cerr << "TRA_data_pkt_gen_event error: no node " << src << "!" << endl;
        return ;
    }
    else if ( dst != BROCAST_ID && node::id_to_node(dst) == nullptr ) {
        cerr << "TRA_data_pkt_gen_event error: no node " << dst << "!" << endl;
        return;
    }
    
    TRA_data_packet *pkt = dynamic_cast<TRA_data_packet*> ( packet::packet_generator::generate("TRA_data_packet") );
    if (pkt == nullptr) { 
        cerr << "packet type is incorrect" << endl; return; 
    }
    TRA_data_header *hdr = dynamic_cast<TRA_data_header*> ( pkt->getHeader() );
    TRA_data_payload *pld = dynamic_cast<TRA_data_payload*> ( pkt->getPayload() );
    
    if (hdr == nullptr) {
        cerr << "header type is incorrect" << endl; return ;
    }
    if (pld == nullptr) {
        cerr << "payload type is incorrect" << endl; return ;
    }

    hdr->setSrcID(src);
    hdr->setDstID(dst);
    hdr->setPreID(src); // this column is not important when the packet is first received by the src (i.e., just generated)
    hdr->setNexID(src); // this column is not important when the packet is first received by the src (i.e., just generated)

    pld->setMsg(msg);
    
    recv_event::recv_data e_data;
    e_data.s_id = src;
    e_data.r_id = src; // to make the packet start from the src
    e_data._pkt = pkt;
    
    recv_event *e = dynamic_cast<recv_event*> ( event::event_generator::generate("recv_event", trigger_time, (void *)&e_data) );

}
unsigned int TRA_data_pkt_gen_event::event_priority() const {
    string string_for_hash;
    string_for_hash = to_string(getTriggerTime()) + to_string(src) + to_string (dst) ; //to_string (pkt->getPacketID());
    return get_hash_value(string_for_hash);
}
// the TRA_data_pkt_gen_event::print() function is used for log file
void TRA_data_pkt_gen_event::print () const {
    cout << "time "          << setw(11) << event::getCurTime() 
         << "        "       << setw(11) << " "
         << "        "       << setw(11) << " "
         << "   srcID"       << setw(11) << src
         << "   dstID"       << setw(11) << dst
         << "        "       << setw(11) << " "
         << "        "       << setw(11) << " "
         << "   TRA_data_packet generating"
         << endl;
}

class TRA_ctrl_pkt_gen_event: public event {
    public:
        class gen_data; // forward declaration
            
    private:
        TRA_ctrl_pkt_gen_event (TRA_ctrl_pkt_gen_event &){}
        TRA_ctrl_pkt_gen_event (){} // we don't allow users to new a recv_event by themselves
        // this constructor cannot be directly called by users; only by generator
        unsigned int src; // the src
        unsigned int dst; // the dst 
        // unsigned int mat;
        // unsigned int act;
        // packet *pkt; // the packet
        string msg;
        // double per; // percentage
    
    protected:
        TRA_ctrl_pkt_gen_event (unsigned int _trigger_time, void *data): event(_trigger_time), src(BROCAST_ID), dst(BROCAST_ID){
            pkt_gen_data * data_ptr = (pkt_gen_data*) data;
            src = data_ptr->src_id;
            dst = data_ptr->dst_id;
            // pkt = data_ptr->_pkt;
            // mat = data_ptr->mat_id;
            // act = data_ptr->act_id;
            msg = data_ptr->msg;
            // per = data_ptr->per;
        } 
        
    public:
        virtual ~TRA_ctrl_pkt_gen_event(){}
        // TRA_ctrl_pkt_gen_event will trigger the packet gen function
        virtual void trigger();
        
        unsigned int event_priority() const;
        
        class TRA_ctrl_pkt_gen_event_generator;
        friend class TRA_ctrl_pkt_gen_event_generator;
        // TRA_ctrl_pkt_gen_event_generator is derived from event_generator to generate an event
        class TRA_ctrl_pkt_gen_event_generator : public event_generator{
                static TRA_ctrl_pkt_gen_event_generator sample;
                // this constructor is only for sample to register this event type
                TRA_ctrl_pkt_gen_event_generator() { /*cout << "send_event registered" << endl;*/ register_event_type(&sample); }
            protected:
                virtual event * generate(unsigned int _trigger_time, void *data){ 
                    // cout << "send_event generated" << endl; 
                    return new TRA_ctrl_pkt_gen_event(_trigger_time, data); 
                }
            
            public:
                virtual string type() { return "TRA_ctrl_pkt_gen_event";}
                ~TRA_ctrl_pkt_gen_event_generator(){}
        };
        // this class is used to initialize the TRA_ctrl_pkt_gen_event
        class pkt_gen_data{
            public:
                unsigned int src_id; // the controller
                unsigned int dst_id; // the node that should update its rule
                // unsigned int mat_id; // the target of the rule
                // unsigned int act_id; // the next hop toward the target recorded in the rule
                string msg;
                // double per; // the percentage
                // packet *_pkt;
        };
        
        void print () const;
};
TRA_ctrl_pkt_gen_event::TRA_ctrl_pkt_gen_event_generator TRA_ctrl_pkt_gen_event::TRA_ctrl_pkt_gen_event_generator::sample;

void TRA_ctrl_pkt_gen_event::trigger() {
    
    TRA_ctrl_packet *pkt = dynamic_cast<TRA_ctrl_packet*> ( packet::packet_generator::generate("TRA_ctrl_packet") );
    if (pkt == nullptr) { 
        cerr << "packet type is incorrect" << endl; return; 
    }
    TRA_ctrl_header *hdr = dynamic_cast<TRA_ctrl_header*> ( pkt->getHeader() );
    TRA_ctrl_payload *pld = dynamic_cast<TRA_ctrl_payload*> ( pkt->getPayload() );
    
    if (hdr == nullptr) {
        cerr << "header type is incorrect" << endl; return ;
    }
    if (pld == nullptr) {
        cerr << "payload type is incorrect" << endl; return ;
    }

    hdr->setSrcID(src); 
    hdr->setDstID(dst);
    hdr->setPreID(src);
    // hdr->setNexID(dst); // in hw3, you should set NexID to src
    hdr->setNexID(src);
    
    // payload
    pld->setMsg(msg);
    // pld->setMatID(mat);
    // pld->setActID(act);
    // pld->setPer(per);
    
    recv_event::recv_data e_data;
    e_data.s_id = src;
    // e_data.r_id = dst; // in hw3, you should set r_id it src
    e_data.r_id = src;
    e_data._pkt = pkt;
    
    recv_event *e = dynamic_cast<recv_event*> ( event::event_generator::generate("recv_event",trigger_time, (void *)&e_data) );
}
unsigned int TRA_ctrl_pkt_gen_event::event_priority() const {
    string string_for_hash;
    // string_for_hash = to_string(getTriggerTime()) + to_string(src) + to_string(dst) + to_string(mat) + to_string(act); //to_string (pkt->getPacketID());
    string_for_hash = to_string(getTriggerTime()) + to_string(src) + to_string(dst) ; //to_string (pkt->getPacketID());
    return get_hash_value(string_for_hash);
}
// the TRA_ctrl_pkt_gen_event::print() function is used for log file
void TRA_ctrl_pkt_gen_event::print () const {
    cout << "time "          << setw(11) << event::getCurTime() 
         << "        "       << setw(11) << " "
         << "        "       << setw(11) << " "
         << "   srcID"       << setw(11) << src
         << "   dstID"       << setw(11) << dst
         << "        "       << setw(11) << " "
         << "        "       << setw(11) << " "
        //  << "   matID"       << setw(11) << mat
        //  << "   actID"       << setw(11) << act
         << "   TRA_ctrl_packet generating"
         << endl;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

class SDN_ctrl_pkt_gen_event: public event {
    public:
        class gen_data; // forward declaration
            
    private:
        SDN_ctrl_pkt_gen_event (SDN_ctrl_pkt_gen_event &){}
        SDN_ctrl_pkt_gen_event (){} // we don't allow users to new a recv_event by themselves
        // this constructor cannot be directly called by users; only by generator
        unsigned int src; // the src
        unsigned int dst; // the dst 
        unsigned int mat;
        unsigned int act;
        // packet *pkt; // the packet
        string msg;
        double per; // percentage
    
    protected:
        SDN_ctrl_pkt_gen_event (unsigned int _trigger_time, void *data): event(_trigger_time), src(BROCAST_ID), dst(BROCAST_ID){
            pkt_gen_data * data_ptr = (pkt_gen_data*) data;
            src = data_ptr->src_id;
            dst = data_ptr->dst_id;
            // pkt = data_ptr->_pkt;
            mat = data_ptr->mat_id;
            act = data_ptr->act_id;
            msg = data_ptr->msg;
            per = data_ptr->per;
        } 
        
    public:
        virtual ~SDN_ctrl_pkt_gen_event(){}
        // SDN_ctrl_pkt_gen_event will trigger the packet gen function
        virtual void trigger();
        
        unsigned int event_priority() const;
        
        class SDN_ctrl_pkt_gen_event_generator;
        friend class SDN_ctrl_pkt_gen_event_generator;
        // SDN_ctrl_pkt_gen_event_generator is derived from event_generator to generate an event
        class SDN_ctrl_pkt_gen_event_generator : public event_generator{
                static SDN_ctrl_pkt_gen_event_generator sample;
                // this constructor is only for sample to register this event type
                SDN_ctrl_pkt_gen_event_generator() { /*cout << "send_event registered" << endl;*/ register_event_type(&sample); }
            protected:
                virtual event * generate(unsigned int _trigger_time, void *data){ 
                    // cout << "send_event generated" << endl; 
                    return new SDN_ctrl_pkt_gen_event(_trigger_time, data); 
                }
            
            public:
                virtual string type() { return "SDN_ctrl_pkt_gen_event";}
                ~SDN_ctrl_pkt_gen_event_generator(){}
        };
        // this class is used to initialize the SDN_ctrl_pkt_gen_event
        class pkt_gen_data{
            public:
                unsigned int src_id; // the controller
                unsigned int dst_id; // the node that should update its rule
                unsigned int mat_id; // the target of the rule
                unsigned int act_id; // the next hop toward the target recorded in the rule
                string msg;
                double per; // the percentage
                // packet *_pkt;
        };
        
        void print () const;
};
SDN_ctrl_pkt_gen_event::SDN_ctrl_pkt_gen_event_generator SDN_ctrl_pkt_gen_event::SDN_ctrl_pkt_gen_event_generator::sample;

void SDN_ctrl_pkt_gen_event::trigger() {
    if ( dst == BROCAST_ID || node::id_to_node(dst) == nullptr ) {
        cerr << "SDN_ctrl_pkt_gen_event error: no node " << dst << "!" << endl;
        return;
    }
    
    SDN_ctrl_packet *pkt = dynamic_cast<SDN_ctrl_packet*> ( packet::packet_generator::generate("SDN_ctrl_packet") );
    if (pkt == nullptr) { 
        cerr << "packet type is incorrect" << endl; return; 
    }
    SDN_ctrl_header *hdr = dynamic_cast<SDN_ctrl_header*> ( pkt->getHeader() );
    SDN_ctrl_payload *pld = dynamic_cast<SDN_ctrl_payload*> ( pkt->getPayload() );
    
    if (hdr == nullptr) {
        cerr << "header type is incorrect" << endl; return ;
    }
    if (pld == nullptr) {
        cerr << "payload type is incorrect" << endl; return ;
    }

    hdr->setSrcID(src); 
    hdr->setDstID(dst);
    hdr->setPreID(src);
    // hdr->setNexID(dst); // in hw3, you should set NexID to src
    hdr->setNexID(src);
    
    // payload
    pld->setMsg(msg);
    pld->setMatID(mat);
    pld->setActID(act);
    pld->setPer(per);
    
    recv_event::recv_data e_data;
    e_data.s_id = src;
    // e_data.r_id = dst; // in hw3, you should set r_id it src
    e_data.r_id = src;
    e_data._pkt = pkt;
    
    recv_event *e = dynamic_cast<recv_event*> ( event::event_generator::generate("recv_event",trigger_time, (void *)&e_data) );
}
unsigned int SDN_ctrl_pkt_gen_event::event_priority() const {
    string string_for_hash;
    string_for_hash = to_string(getTriggerTime()) + to_string(src) + to_string(dst) + to_string(mat) + to_string(act); //to_string (pkt->getPacketID());
    return get_hash_value(string_for_hash);
}
// the SDN_ctrl_pkt_gen_event::print() function is used for log file
void SDN_ctrl_pkt_gen_event::print () const {
    cout << "time "          << setw(11) << event::getCurTime() 
         << "        "       << setw(11) << " "
         << " percent"       << setw(11) << per
         << "   srcID"       << setw(11) << src 
         << "   dstID"       << setw(11) << dst
         << "   matID"       << setw(11) << mat
         << "   actID"       << setw(11) << act
         << "   SDN_ctrl_packet generating"
         << endl;
}


////////////////////////////////////////////////////////////////////////////////

class link {
        // all links created in the program
        static map< pair<unsigned int,unsigned int>, link*> id_id_link_table;
        
        unsigned int id1; // from
        unsigned int id2; // to
        
    protected:
        link(link&){} // this constructor should not be used
        link(){} // this constructor should not be used
        link(unsigned int _id1, unsigned int _id2): id1(_id1), id2(_id2) { id_id_link_table[pair<unsigned int,unsigned int>(id1,id2)] = this; }

    public:
        virtual ~link() { 
            id_id_link_table.erase (pair<unsigned int,unsigned int>(id1,id2)); // erase the link
        }
        
        static link * id_id_to_link (unsigned int _id1, unsigned int _id2) { 
            return ((id_id_link_table.find(pair<unsigned int,unsigned int>(_id1,_id2))!=id_id_link_table.end()) ? id_id_link_table[pair<unsigned,unsigned>(_id1,_id2)]: nullptr) ; 
        }

        virtual double getLatency() = 0; // you must implement your own latency
        
        static void del_link (unsigned int _id1, unsigned int _id2) {
            pair<unsigned int, unsigned int> temp;
            if (id_id_link_table.find(temp)!=id_id_link_table.end())
                id_id_link_table.erase(temp); 
        }

        static unsigned int getLinkNum () { return id_id_link_table.size(); }

        class link_generator {
                // lock the copy constructor
                link_generator(link_generator &){}
                // store all possible types of link
                static map<string,link_generator*> prototypes;
            protected:
                // allow derived class to use it
                link_generator() {}
                // after you create a new link type, please register the factory of this link type by this function
                void register_link_type(link_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your link
                virtual link* generate(unsigned int _id1, unsigned int _id2) = 0;
            public:
                // you have to implement your own type() to return your link type
        	    virtual string type() = 0;
        	    // this function is used to generate any type of link derived
        	    static link * generate (string type, unsigned int _id1, unsigned int _id2) {
        	        if(id_id_link_table.find(pair<unsigned int,unsigned int>(_id1,_id2))!=id_id_link_table.end()){
        	            std::cerr << "duplicate link id" << std::endl; // link id is duplicated
        	            return nullptr;
        	        }
        	        else if ( BROCAST_ID == _id1 || BROCAST_ID == _id2 ) {
        	            cerr << "BROCAST_ID cannot be used" << endl;
        	            return nullptr;
        	        }
            		else if (prototypes.find(type) != prototypes.end()){ // if this type derived exists 
            		    link * created_link = prototypes[type]->generate(_id1,_id2);
            			return created_link; // generate it!!
            		}
            		std::cerr << "no such link type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered link types: " << endl;
            	    for (map<string,link::link_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~link_generator(){};
        };
};
map<string,link::link_generator*> link::link_generator::prototypes;
map<pair<unsigned int,unsigned int>, link*> link::id_id_link_table;

void node::add_phy_neighbor (unsigned int _id, string link_type){
    if (id == _id) return; // if the two nodes are the same...
    if (id_node_table.find(_id)==id_node_table.end()) return; // if this node does not exist
    if (phy_neighbors.find(_id)!=phy_neighbors.end()) return; // if this neighbor has been added
    phy_neighbors[_id] = true;
    
    link::link_generator::generate(link_type,id,_id);
}
void node::del_phy_neighbor (unsigned int _id){
    phy_neighbors.erase(_id);
    
}


class simple_link: public link {
    protected:
        simple_link() {} // it should not be used outside the class
        simple_link(simple_link&) {} // it should not be used
        simple_link(unsigned int _id1, unsigned int _id2): link (_id1,_id2){} // this constructor cannot be directly called by users
    
    public:
        virtual ~simple_link() {}
        virtual double getLatency() { return ONE_HOP_DELAY; } // you can implement your own latency
        
        class simple_link_generator;
        friend class simple_link_generator;
        // simple_link is derived from link_generator to generate a link
        class simple_link_generator : public link_generator {
                static simple_link_generator sample;
                // this constructor is only for sample to register this link type
                simple_link_generator() { /*cout << "simple_link registered" << endl;*/ register_link_type(&sample); }
            protected:
                virtual link * generate(unsigned int _id1, unsigned int _id2) 
                { /*cout << "simple_link generated" << endl;*/ return new simple_link(_id1,_id2); }
            public:
                virtual string type() { return "simple_link"; }
                ~simple_link_generator(){}
        };
};

simple_link::simple_link_generator simple_link::simple_link_generator::sample;


// the data_packet_event function is used to add an initial event
void data_packet_event (unsigned int src, unsigned int dst, unsigned int t = 0, string msg = "default"){
    if ( node::id_to_node(src) == nullptr || (dst != BROCAST_ID && node::id_to_node(dst) == nullptr) ) {
        cerr << "src or dst is incorrect" << endl; return ;
        return;
    }

    TRA_data_pkt_gen_event::pkt_gen_data e_data;
    e_data.src_id = src;
    e_data.dst_id = dst;
    e_data.msg = msg;
    
    // recv_event *e = dynamic_cast<recv_event*> ( event::event_generator::generate("recv_event",t, (void *)&e_data) );
    TRA_data_pkt_gen_event *e = dynamic_cast<TRA_data_pkt_gen_event*> ( event::event_generator::generate("TRA_data_pkt_gen_event",t, (void *)&e_data) );
    if (e == nullptr) cerr << "event type is incorrect" << endl;
}

// the TRA_ctrl_packet_event function is used to add an initial event
void TRA_ctrl_packet_event (unsigned int src, unsigned int t = event::getCurTime(),
                        string msg = "default") {
        // 1st parameter: the source; the destination that want to broadcast a msg with counter 0 (i.e., match ID)
        // 2nd parameter: time (optional)
        // 3rd parameter: msg for debug information (optional)
    if ( node::id_to_node(src) == nullptr ) {
        cerr << "id is incorrect" << endl; return;
    }
    
    // unsigned int src = con_id;
    TRA_ctrl_pkt_gen_event::pkt_gen_data e_data;
    e_data.src_id = src;
    e_data.dst_id = BROCAST_ID;
    // e_data.mat_id = mat;
    // e_data.act_id = act;
    e_data.msg = msg;
    // e_data.per = per;
    
    TRA_ctrl_pkt_gen_event *e = dynamic_cast<TRA_ctrl_pkt_gen_event*> ( event::event_generator::generate("TRA_ctrl_pkt_gen_event",t, (void *)&e_data) );
    if (e == nullptr) cerr << "event type is incorrect" << endl;
}

// the SDN_ctrl_packet_event function is used to add an initial event
void SDN_ctrl_packet_event (unsigned int con_id, unsigned int id, 
                        unsigned int mat, unsigned int act, 
                        double per, unsigned int t = event::getCurTime(),
                        string msg = "default") {
        // 1st parameter: the controller id
        // 2nd parameter: the id of node that has to update the table
        // 3rd parameter: the destination id (i.e., match ID)
        // 4th parameter: the next-hop node toward the target node recorded in the rule (i.e., action ID)
        // 5th parameter: the percentage (e.g., 45%: 0.45)
        // 6th parameter: time (optional)
        // 7th parameter: msg for debug information (optional)
    if ( id == BROCAST_ID || node::id_to_node(id) == nullptr ) {
        cerr << "id is incorrect" << endl; return;
    }
    
    unsigned int src = con_id;
    SDN_ctrl_pkt_gen_event::pkt_gen_data e_data;
    e_data.src_id = src;
    e_data.dst_id = id;
    e_data.mat_id = mat;
    e_data.act_id = act;
    e_data.msg = msg;
    e_data.per = per;
    
    SDN_ctrl_pkt_gen_event *e = dynamic_cast<SDN_ctrl_pkt_gen_event*> ( event::event_generator::generate("SDN_ctrl_pkt_gen_event",t, (void *)&e_data) );
    if (e == nullptr) cerr << "event type is incorrect" << endl;
}

// send_handler function is used to transmit packet p based on the information in the header
// Note that the packet p will not be discard after send_handler ()

void node::send_handler(packet *p){
    packet *_p = packet::packet_generator::replicate(p);
    send_event::send_data e_data;
    e_data.s_id = _p->getHeader()->getPreID();
    e_data.r_id = _p->getHeader()->getNexID();
    e_data._pkt = _p;
    send_event *e = dynamic_cast<send_event*> (event::event_generator::generate("send_event",event::getCurTime(), (void *)&e_data) );
    if (e == nullptr) cerr << "event type is incorrect" << endl;
}

void node::send(packet *p){ // this function is called by event; not for the user
    if (p == nullptr) return;
    
    unsigned int _nexID = p->getHeader()->getNexID();
    for ( map<unsigned int,bool>::iterator it = phy_neighbors.begin(); it != phy_neighbors.end(); it ++) {
        unsigned int nb_id = it->first; // neighbor id
        
        if (nb_id != _nexID && BROCAST_ID != _nexID) continue; // this neighbor will not receive the packet
        
        unsigned int trigger_time = event::getCurTime() + link::id_id_to_link(id, nb_id)->getLatency() ; // we simply assume that the delay is fixed
        // cout << "node " << id << " send to node " <<  nb_id << endl;
        recv_event::recv_data e_data;
        e_data.s_id = id;    // set the sender   (i.e., preID)
        e_data.r_id = nb_id; // set the receiver (i.e., nexID)
        
        packet *p2 = packet::packet_generator::replicate(p);
        e_data._pkt = p2;
        
        recv_event *e = dynamic_cast<recv_event*> (event::event_generator::generate("recv_event", trigger_time, (void*) &e_data)); // send the packet to the neighbor
        if (e == nullptr) cerr << "event type is incorrect" << endl;
    }
    packet::discard(p);
}

// you have to write the code in recv_handler of TRA_switch
void TRA_switch::recv_handler (packet *p){
    // in this function, you are "not" allowed to use node::id_to_node(id) !!!!!!!!

    // this is a simple example
    // node 0 broadcasts its message to every node and every node relays the packet "only once" and increases its counter
    // the variable hi is used to examine whether the packet has been received by this node before
    // you can remove the variable hi and create your own routing table in class TRA_switch
    if (p == nullptr) return ;
    
    if (p->type() == "TRA_ctrl_packet") { // the switch receives a packet from the controller
        TRA_ctrl_packet *p3 = nullptr;
        p3 = dynamic_cast<TRA_ctrl_packet*> (p);
        TRA_ctrl_payload *l3 = nullptr;
        l3 = dynamic_cast<TRA_ctrl_payload*> (p3->getPayload());
        
        unsigned int src_id = p3->getHeader()->getSrcID(); // it will be dst in routing table, but when OSPF it will be src
        unsigned int pre_id = p3->getHeader()->getPreID(); // it will br next node of path in routing table, but when OSPF it will be pre(last) node
        int pre_counter = l3->getCounter(); //the counter of the package sent from last node 
        bool send_button = false; // decide whether to send package

        if(!Is_exist_turn(src_id)) { // if first time receive packet , it will be !false (becaseue we may have multiple dst , so I set turn in TRA_switch)
            Set_turn(src_id);
            Set_parent_counter(src_id, pre_counter);
            Create_map(src_id , pre_id); // in routing table is ( [dst] : next_node )
            send_button = true;
        } else if(pre_counter < Get_parent_counter(src_id)) { // if pre_counter smaller than origin parent counter
            Set_parent_counter(src_id, pre_counter);
            Create_map(src_id , pre_id);
            send_button = true;
        } else if(pre_counter == Get_parent_counter(src_id)) { // id small
            unsigned int origin_id = Get_Id_From_Table(src_id);
            if(pre_id < origin_id) {
                Create_map(src_id , pre_id);
                send_button = true;
            }
        }

        if(send_button) {
            p3->getHeader()->setPreID ( getNodeID() );
            p3->getHeader()->setNexID ( BROCAST_ID );
            p3->getHeader()->setDstID ( BROCAST_ID );
            l3->increase();
            send_handler(p3);
        }

        // unsigned mat = l3->getMatID();
        // unsigned act = l3->getActID();
        // string msg = l3->getMsg(); // get the msg
    }
    else if (p->type() == "TRA_data_packet" ) { // the switch receives a packet
      return;
    }
    
    // you should define your own class SDN_switch to receive the mat, act, per information
    // if (p->type() == "SDN_ctrl_packet" ) {
    //     SDN_ctrl_packet *p3 = nullptr;
    //     p3 = dynamic_cast<SDN_ctrl_packet*> (p);
    //     SDN_ctrl_payload *l3 = nullptr;
    //     l3 = dynamic_cast<SDN_ctrl_payload*> (p3->getPayload());
        
    //     p3->getHeader()->setPreID ( getNodeID() );
    //     p3->getHeader()->setNexID ( 2 );
    //     // p3->getHeader()->setDstID (  );
    //     send_handler(p3);
    // }

    // you should implement the OSPF algorithm in recv_handler
    // getNodeID() returns the id of the current node
    
    // The current node's neighbors are already stored in the following variable 
    // map<unsigned int,bool> node::phy_neighbors
    // however, this variable is private in the class node
    // You have to use node::getPhyNeighbors to get the variable
    // for example, if you want to print all the neighbors of this node
    // const map<unsigned int,bool> &nblist = getPhyNeighbors();
    // cout << "node " << getNodeID() << "'s nblist: ";
    // for (map<unsigned int,bool>::const_iterator it = nblist.begin(); it != nblist.end(); it ++) {
    //     cout << it->first << ", " ;
    // }
    // cout << endl;


    // you can use p->getHeader()->setSrcID() or getSrcID()
    //             p->getHeader()->setDstID() or getDstID()
    //             p->getHeader()->setPreID() or getPreID()
    //             p->getHeader()->setNexID() or getNexID() to change or read the packet header
    
    // In addition, you can get the packet, header, and payload with the correct type (GR)
    // in fact, this is downcasting
    // TRA_data_packet * pkt = dynamic_cast<TRA_data_packet*> (p);
    // TRA_data_header * hdr = dynamic_cast<TRA_data_header*> (p->getHeader());
    // TRA_data_payload * pld = dynamic_cast<TRA_data_payload*> (p->getPayload());
    
    // you can also change the TRA_data_payload setting
    // pld->setMsg(string): to set the message transmitted to the destination
    
    // Besides, you can use packet::packet_generator::generate() to generate a new packet; note that you should fill the header and payload in the packet
    // moreover, you can use "packet *p2 = packet::packet_generator::replicate(p)" to make a clone p2 of packet p
    // note that if the packet is generated or replicated manually, you must delete it by packet::discard() manually before recv_handler finishes
    
    // "IMPORTANT":
    // You have to "carefully" fill the correct information (e.g., srcID, dstID, ...) in the packet before you send it
    // Note that if you want to transmit a packet to only one next node (i.e., unicast), then you fill the ID of a specific node to "nexID" in the header
    // Otherwise, i.e., you want to broadcasts, then you fill "BROCAST_ID" to "nexID" in the header
    // after that, you can use send() to transmit the packet 
    // usage: send_handler (p);
    
    // note that packet p will be discarded (deleted) after recv_handler(); you don't need to manually delete it
}
void SDN_switch::recv_handler (packet *p){
  if (p == nullptr) return ;
  
  if (p->type() == "SDN_ctrl_packet") { // the switch receives a packet from the controller
    SDN_ctrl_packet *p3 = nullptr;
    p3 = dynamic_cast<SDN_ctrl_packet*> (p);
    SDN_ctrl_payload *l3 = nullptr;
    l3 = dynamic_cast<SDN_ctrl_payload*> (p3->getPayload());
    
        
    // In SDN_ctrl_pkt_gen_event::trigger()
    //--------------get data--------------------------------
    unsigned int con_id = p3->getHeader()->getSrcID(); // also can use getPreID()
    unsigned int sdn_id = p3->getHeader()->getDstID();
    //-------------------------------------------------
    unsigned int dst_id = l3->getMatID();
    unsigned int next_hop = l3->getActID();
    double ratio = l3->getPer();
    //---------------------------------------------
    
    setSdnTable(dst_id, next_hop, ratio);

    //---------------send package-------------------------

  }
}
void SDN_controller::recv_handler (packet *p){
  if (p == nullptr) return ;
  
  if (p->type() == "SDN_ctrl_packet") { // the switch receives a packet from the controller
      SDN_ctrl_packet *p3 = nullptr;
      p3 = dynamic_cast<SDN_ctrl_packet*> (p);
      SDN_ctrl_payload *l3 = nullptr;
      l3 = dynamic_cast<SDN_ctrl_payload*> (p3->getPayload());
      
      
      // In SDN_ctrl_pkt_gen_event::trigger()
      //--------------get data--------------------------------
      unsigned int con_id = p3->getHeader()->getSrcID();
      unsigned int sdn_id = p3->getHeader()->getDstID();
      //-------------------------------------------------
      unsigned int dst_id = l3->getMatID();
      unsigned int next_hop = l3->getActID();
      double ratio = l3->getPer();
      //---------------------------------------------
      
      //-------------set send data----------------------------
      // p3->getHeader()->setSrcID (); // no need to change
      // p3->getHeader()->setDstID ( dst_id ); // the same
      p3->getHeader()->setPreID (getNodeID());
      p3->getHeader()->setNexID (sdn_id);
      
      l3->setMatID(dst_id);
      l3->setActID(next_hop);
      l3->setPer(ratio);

      send_handler(p3);

      // send_handler(p3);
      // unsigned mat = l3->getMatID();
      // unsigned act = l3->getActID();
      // string msg = l3->getMsg(); // get the msg
  }  
}

unsigned int* BFS(const unsigned int dst_id,const int nodes, const vector<vector<bool>> &link) { 

  bool *visited = new bool[nodes];
  unsigned int *next_node = new unsigned int[nodes];  // because BFS begin with destination , so pre node in BFS will be the next node actually in routing table

  for(int i = 0; i < nodes; i++) {
    visited[i] = false;
    next_node[i] = nodes + 3;
  }

  queue<unsigned int> q;
  unsigned int current_node;

  visited[dst_id] = true; // begin with dst
  q.push(dst_id);
  next_node[dst_id] = dst_id;

  while(!q.empty()) {
    current_node = q.front();
    q.pop();

    for(unsigned int neighbor_id = 0; neighbor_id < nodes; neighbor_id++) {
      if(link[current_node][neighbor_id] == false) continue;
      if(!visited[neighbor_id]){
        visited[neighbor_id] = true;
        next_node[neighbor_id] = current_node;
        q.push(neighbor_id);
      }  
    }
  }

  delete [] visited;
  return next_node;
}

bool cmp(const pair<unsigned int, int> &p1,const pair<unsigned int, int> &p2) {
	return p1.second > p2.second;
}

//-----------------------------------------------------

int** generate_residual(const unsigned int nodes, const unsigned int dst_id, unsigned int* next_node,const vector<vector<unsigned int>>& flow_list, int weight) {

  // I set a virtual node to connect all src in flow
  int total_node = nodes + 1; 
  
  // new a residual flow matrix (2-D)
  int **link_weight = new int*[total_node];
  for(int row = 0; row < total_node; row++) {
    link_weight[row] = new int[total_node];
  }

  // initiate
  for(int row = 0; row < total_node; row++) {
    for(int column = 0; column < total_node; column++) {
      link_weight[row][column] = 0;
    }
  }
  
  // nodes - 1
  // residual flow matrix (not include virtual node)
  int next_id;
  for(int i = 0; i < nodes ; i++) {
    if(i == dst_id) continue;
    next_id = next_node[i];
    link_weight[i][next_id] = weight; // ?
  }
  

  // nodes
  // virtual node
  // flow_list 0:src , 1:dst, 2:flow_weight
  for(int row = 0; row < flow_list.size(); row++) {
    int src = flow_list[row][0];
    link_weight[nodes][src] = flow_list[row][2];
  }
 
  return link_weight;
}

bool bfs(int** residual_weight, int src_id, int dst_id, int parent [], int total_node_num) {

  // bool *visited = new bool[total_node_num];
  bool visited[5000];
  for(int i = 0; i < total_node_num; i++) {
    visited[i] = false;
  }

  queue<int> q;
  q.push(src_id);
  visited[src_id] = true;
  parent[src_id] = -1;

  while(!q.empty()) {
    int current_node_id = q.front();
    q.pop();

    for(int neighbor_id = 0; neighbor_id < total_node_num; neighbor_id++) {
      if(visited[neighbor_id] == false && residual_weight[current_node_id][neighbor_id] > 0) {
        if(neighbor_id == dst_id) {
          parent[neighbor_id] = current_node_id;
          return true; // because we only want to find a path
        }

        q.push(neighbor_id);
        parent[neighbor_id] = current_node_id;
        visited[neighbor_id] = true;
      }
    }
  }
  return false;
}

int ford_fulkerson(const unsigned int dst_id,const int nodes, int** residual_weight, const vector<vector<unsigned int>>& flow_list) {

  int total_node_num = nodes + 1;
  vector<vector<bool>> reverse_record(total_node_num, vector<bool>(total_node_num, false));
  int * parent = new int[total_node_num];
  int max_flow = 0;
  int src_id = nodes;
  while(bfs(residual_weight, nodes, dst_id, parent, total_node_num)) { // find a path from src to dst 
    int path_flow = INT_MAX;
    for(int node_id = dst_id; node_id != src_id; node_id = parent[node_id]) {
      int last_id = parent[node_id];
      path_flow = min(path_flow, residual_weight[last_id][node_id]);
    }
    for(int node_id = dst_id; node_id != src_id; node_id = parent[node_id]) {
      int last_id = parent[node_id];
      residual_weight[last_id][node_id] -= path_flow;
      // this make bfs will change continuously
      residual_weight[node_id][last_id] += path_flow; // construct a reverse edge
      reverse_record[node_id][last_id] = true;
    }
    max_flow += path_flow;
  }  
  for(int row = 0; row < total_node_num; row++) {
    for(int column = 0; column < total_node_num; column++) {
      if(reverse_record[row][column] == true) {
        residual_weight[row][column] = 0;
      }
    }
  }
  return max_flow;

}

bool mysort(const vector<int> &v1, const vector<int> &v2) {
	return v1[0] > v2[0];
}

// void add_sdn_weight() {

// }


int main()
{
    // header::header_generator::print(); // print all registered headers
    // payload::payload_generator::print(); // print all registered payloads
    // packet::packet_generator::print(); // print all registered packets
    // node::node_generator::print(); // print all registered nodes
    // event::event_generator::print(); // print all registered events
    // link::link_generator::print(); // print all registered links
    
    
    // read the input
    // ------------------------------------------
    unsigned int nodes, dsts, links, pairs, simtime, sdn_ctrl_time, budget;
    cin >> nodes >> dsts >> links >> pairs >> simtime >> sdn_ctrl_time >> budget;

    // dst
    vector <unsigned int> dstid_list(dsts);
    for(auto &dstid: dstid_list) cin >> dstid;
    
    // cost & broadcast time
    unsigned int node_id;
    int temp_cost, temp_time; 
    map<unsigned int, int> upgrade_cost;
    map<unsigned int, int> broadcast_time;

    for(int i = 0; i < nodes; i++) {
      auto it = find(dstid_list.begin(), dstid_list.end(), i);
      if(it != dstid_list.end()) { // find dst
        cin >> node_id >> temp_cost >> temp_time;
        upgrade_cost[node_id] = temp_cost;
        broadcast_time[node_id] = temp_time;
      } else {
        cin >> node_id >> temp_cost;
        upgrade_cost[node_id] = temp_cost;
      }
    }

    // link
    vector<vector<bool>> link(nodes, vector<bool>(nodes, false)); // (links, vector<unsigned int>(2, 0))
    unsigned int temp_id, node1, node2;
    for(int i = 0; i < links; i++) {
      cin >> temp_id >> node1 >> node2;
      link[node1][node2] = true;
      link[node2][node1] = true;
    }

    // flow
    unsigned int src, dst, flow_size;
    vector<vector<unsigned int>> flow_list; // (pairs, vector<unsigned int>(4,0));

    int total_flow = 0;
    for(int flow_id = 0; flow_id < pairs; flow_id++) {
        cin >> temp_id >> src >> dst >> flow_size;
        vector<unsigned int> temp;
        temp.push_back(src);
        temp.push_back(dst);
        temp.push_back(flow_size);
        total_flow += flow_size;
        flow_list.push_back(temp);
    }
    //----------------------------------------------------

    //---------------caculate sdn node--------------------- 
    
    //------------------caculate sdn node-------------------------------------------    

    unsigned int *next_node; // index is node id , value is the next node of path
    unsigned int sdn_node_id;

    vector<unsigned int> upgraded_node;
    int turn = 0;
    vector<vector<double>> sdn_table_global;


    for(int dst_ids = 0; dst_ids < dsts; dst_ids++) {
      unsigned int dst_id = dstid_list[dst_ids];
      next_node = BFS(dst_id, nodes, link); 
          
      // --------- make a compare base && select nodes to upgrade----------------------
      // has direction
      int ** residual_weight = generate_residual(nodes, dst_id, next_node,flow_list, 5000);
      int ** capacity = generate_residual(nodes, dst_id, next_node,flow_list, 5000);
      vector<vector<int>> actual_flow(nodes+1, vector<int>(nodes+1, -1));
        
      ford_fulkerson(dst_id, nodes, residual_weight, flow_list);

      for(int row = 0; row < nodes + 1; row++) {
        for(int column = 0; column < nodes + 1; column++) {
          if(capacity[row][column] == 0) continue;
          actual_flow[row][column] = capacity[row][column] - residual_weight[row][column];
        }
      }

      // select sdn node
      if(turn == 0) {
        
        turn++;
        
        vector<vector<int>> actual_in_order; 
        for(int row = 0; row < nodes; row++) {
          for(int column = 0; column < nodes; column++) {
            if(capacity[row][column] == 0) continue;
            vector<int> temp;
            temp.push_back(actual_flow[row][column]);
            temp.push_back(row);
            temp.push_back(column);
            actual_in_order.push_back(temp);
          }
        }

        int total_cost = 0;
        
        sort(actual_in_order.begin(), actual_in_order.end(), mysort);
        for(int row = 0; row < actual_in_order.size(); row++) {
          int node_id_compare = actual_in_order[row][1];
          if(find(upgraded_node.begin(), upgraded_node.end(), node_id_compare) != upgraded_node.end()) continue;
          int cost_compare = upgrade_cost[node_id_compare];
          if(total_cost + cost_compare <= budget) {
            total_cost += cost_compare;
            upgraded_node.push_back(node_id_compare);
          }
        }
      }

      for(int row = 0; row < nodes + 1; row++) {
        delete [] residual_weight[row];
        delete [] capacity[row];

      }
      
      
      //--------------[a , b]--------------------------------
      int left = 0;
      int right = 5000;
      while(left <= right) {
        int middle = (left + right)/2;
        int ** residual_weight = generate_residual(nodes, dst_id, next_node,flow_list, middle);
        int ** capacity = generate_residual(nodes, dst_id, next_node,flow_list, middle);
        // for(int sdn_id = 0; sdn_id < upgraded_node.size(); sdn_id++) {
        for(auto it = upgraded_node.begin(); it != upgraded_node.end(); it++) {
          for(int column = 0; column < nodes; column++) {
            if(link[*it][column] == true) {
              residual_weight[*it][column] = middle;
            }
          }
        }
        int max_flow = ford_fulkerson(dst_id, nodes, residual_weight, flow_list);
        if(max_flow < total_flow) {
          left = middle + 1;
        } else if(max_flow > total_flow) {
          right = middle - 1;
        } else if(max_flow == total_flow) {
          right = middle - 1;
        }
      }
        // sdn_table_global : [0] sdn_id , [1] dst_id(mat) , [2] next_hop , [3] percentage
      for(auto it = upgraded_node.begin(); it != upgraded_node.end(); it++) {
        vector<double> temp;
        sdn_node_id = *it;
        int actual;
        for(int id = 0; id < nodes; id++) {
          if(link[sdn_node_id][id] == false) continue;
          if((actual = capacity[sdn_node_id][id] - residual_weight[sdn_node_id][id]) != 0) {
              unsigned int next_hop = id;
              temp.push_back(sdn_node_id);
              temp.push_back(dst_id);
              temp.push_back(next_hop);
              temp.push_back(actual/capacity[sdn_node_id][id]);
              sdn_table_global.push_back(temp);
          }
        }       
      }
    }
      
    //---------------generate node (TRA && SDN)-------------------------
    for (unsigned int id = 0; id < nodes; id ++){
        auto it = find(upgraded_node.begin(), upgraded_node.end(), id);
        // if this node is SDN_switch, then create SDN_switch; please define SDN_switch 
        if(it != upgraded_node.end()) {
          node::node_generator::generate("SDN_switch",id);        
        } 
        else { // otherwise, create TRA_switch
          node::node_generator::generate("TRA_switch",id);
        }
    }

    for(int i = 0; i < nodes; i++) {
      for(int j = 0; j < nodes; j++) {
        if(link[i][j] == true) {
          node::id_to_node(i)->add_phy_neighbor(j);
          node::id_to_node(j)->add_phy_neighbor(i);
        }
      }
    }
    //-----------------SDN controller-----------------------------
    unsigned int con_id = node::getNodeNum(); // controller id is node number
    node::node_generator::generate("SDN_controller", con_id);
    for(auto it = upgraded_node.begin(); it != upgraded_node.end(); it++) { // upgrade_node save sdn node
      sdn_node_id = *it;
      node::id_to_node(sdn_node_id)->add_phy_neighbor(con_id);
      node::id_to_node(con_id)->add_phy_neighbor(sdn_node_id);
    }




    //----------------TRA_ctrl_packet_event---------------------------
    for(auto it = broadcast_time.begin(); it != broadcast_time.end(); it++) {
      TRA_ctrl_packet_event((*it).first, (*it).second);
    }
    // 1st parameter: the source; the destination that want to broadcast a msg with counter 0 (i.e., match ID)
    // 2nd parameter: time (optional)
    // 3rd parameter: msg for debug information (optional)
    
    //-------------------SDN_ctrl_packet_event---------------------------
    // node with "con_id" (i.e., controller) sends a packet to node 0 at time 150 with matID 3, actID 4, percentage 35%
    // --> you need to implement SDN_switch, SDN_controller, routing tables for SDN_switch


    // sdn_table_global(2-D) : [0] sdn_id , [1] dst_id(mat) , [2] next_hop , [3] percentage
    for(int row = 0; row < sdn_table_global.size(); row++) {
      SDN_ctrl_packet_event(con_id, sdn_table_global[row][0], sdn_table_global[row][1], sdn_table_global[row][2], sdn_table_global[row][3], sdn_ctrl_time); 
    }
    // 1st parameter: the controller id
    // 2nd parameter: the id of an SDN_switch that has to update the table
    // 3rd parameter: the destination id (i.e., match ID)
    // 4th parameter: the next-hop node toward the target node recorded in the rule (i.e., action ID)
    // 5th parameter: the percentage (e.g., 45%: 0.45)
    // 6th parameter: time (optional)
    // 7th parameter: msg for debug information (optional)

    // start simulation!!
    event::start_simulate(simtime);

    //------------set of sdn tables------------------------------------------
    for(auto it = upgraded_node.begin(); it != upgraded_node.end(); it++) {
      if(it == upgraded_node.begin()) {
        cout << *it;
        continue;
      }
      cout << " " << *it;
    }
    cout << "\n";
    //--------------routing tables----------------------------------------------------
    for(unsigned int id = 0; id < nodes; id++) {
      cout << id << "\n";

      auto it = find(upgraded_node.begin(), upgraded_node.end(), id);
      if(it != upgraded_node.end()) { // sdn node
        SDN_switch * temp_sdn = nullptr;
        temp_sdn = dynamic_cast<SDN_switch *> (node::id_to_node(id));
        for(int j = 0; j < dsts; j++) {
          unsigned int temp_dst_id = dstid_list[j];
          
          int temp_size = temp_sdn->getSize(temp_dst_id);
          cout << temp_dst_id; 

          for(int index = 0; index < temp_size; index++) {
            cout << " " << temp_sdn->getNextHop(temp_dst_id, index) << " " << temp_sdn->getRatio(temp_dst_id, index) * 100 << "%";
          }
          cout << "\n";
        }
      } else {
        TRA_switch * temp_tra = nullptr;
        temp_tra = dynamic_cast<TRA_switch *> (node::id_to_node(id));
        for(int j = 0; j < dsts; j++) {
          cout << dstid_list[j] << " " << temp_tra->routing_table[dstid_list[j]] << "\n";
        }
      }
    }

    // event::flush_events() ;
    // cout << packet::getLivePacketNum() << endl;
    return 0;
}

