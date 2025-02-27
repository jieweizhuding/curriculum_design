#ifndef ARC_NODE_H
#define ARC_NODE_H

#include <string>

namespace AOV{

  class arc_node
  {
  private:
    std::string tind, hind;
    arc_node *tlink=nullptr, *hlink=nullptr;
  public:
    arc_node(){};
    arc_node(std::string t,std::string h)
    {
        this->tind=t;
        this->hind=h;
    }
    std::string get_tind(){
        return tind;
    }
    std::string get_hind(){
        return hind;
    }
    arc_node* get_tlink(){
        return tlink;
    }
    arc_node* get_hlink(){
        return hlink;
    }
    void set_tlink(arc_node* t){
        this->tlink=t;
    }
    void set_hlink(arc_node* h){
        this->hlink=h;
    }
  };
};

#endif
