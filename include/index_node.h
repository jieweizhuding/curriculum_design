#ifndef INDEX_NODE_H
#define INDEX_NODE_H

#include "arc_node.h"

namespace AOV
{
  template <typename VertexInfo>
  class index_node
  {
  private:
    std::string uuid;
    VertexInfo value;
    arc_node *frist_in, *first_out;

  public:
    index_node(){};
    index_node(const std::string s,const VertexInfo &value)
    {
      this->uuid=s;
      this->value = value;
      first_out = nullptr;
      frist_in = nullptr;
    }
    arc_node* get_in(){
        return frist_in;
    }
    arc_node* get_out(){
        return first_out;
    }
    void set_in(arc_node* in){
        this->frist_in=in;
    }
    void set_out(arc_node* out){
        this->first_out=out;
    }
  };

} // namespace AOV

#endif
