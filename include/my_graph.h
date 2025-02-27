#ifndef MY_GRAPH_H
#define MY_GRAPH_H

// C++标准库内容
#include <unordered_map>

// 自定义头文件
#include "index_node.h"

// QT库文件
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsView>

// 三方库文件
#include<ogdf/basic/Graph.h>
#include<ogdf/basic/GraphAttributes.h>

namespace AOV
{
  template <typename VertexInfo>
  class graph
  {
  private:
    // TODO: 需要一个map进行uuid与对象的对应
    std::unordered_map<std::string,AOV::index_node<VertexInfo>> m;

  public:
    std::unordered_map<std::string,AOV::index_node<VertexInfo>> &get_m(){return m;}
    graph() {};
    void draw(QGraphicsView *view);
    bool insert_node(const std::string i,const std::string s);
    void insert_arc(const std::string t,const std::string h);
    void delete_node(const std::string i);
    bool delete_arc(const std::string t,const std::string h);
    ogdf::GraphAttributes switch_graph(ogdf::Graph& g);
  };


  template <typename VertexInfo>
  inline bool graph<VertexInfo>::insert_node(const std::string i,const std::string s)
  {
      if(m.find(i)!=m.end()){
          return false;
      }
    // vector在构造函数中被默认初始化
    index_node<std::string> t(i,s);
    m.insert({i,t});
    return true;
  }

  template<typename VertexInfo>
  inline void graph<VertexInfo>::insert_arc(const std::string t, const std::string h)
  {
      arc_node *arc=new arc_node(t,h);
      arc->set_tlink(m[t].get_out());
      m[t].set_out(arc);
      arc->set_hlink(m[h].get_in());
      m[h].set_in(arc);


  }

  template<typename VertexInfo>
  inline void graph<VertexInfo>::delete_node(const std::string i)
  {
      arc_node* t=new arc_node();
      if(m[i].get_out()==nullptr){
          t->set_tlink(nullptr);
      }else{
          t->set_tlink(m[i].get_out()->get_tlink());
      }
      if(m[i].get_in()==nullptr){
          t->set_hlink(nullptr);
      }else{
          t->set_hlink(m[i].get_in()->get_hlink());
      }
      while (t->get_hlink()!=nullptr&&t->get_tlink()!=nullptr) {
          delete m[i].get_out();
          delete m[i].get_in();
          m[i].set_in(t->get_hlink());
          m[i].set_out(t->get_tlink());
      }
      while(t->get_tlink()!=nullptr){
          delete m[i].get_out();
          m[i].set_out(t->get_tlink());
      }
      while(t->get_hlink()!=nullptr){
          delete m[i].get_in();
          m[i].set_in(t->get_hlink());
      }
      m.erase(i);
      delete t;
  }

  template<typename VertexInfo>
  inline bool graph<VertexInfo>::delete_arc(const std::string tail, const std::string head)
  {
      arc_node* temp;
      arc_node* move=m[tail].get_out();
      if(move==nullptr){
          return false;
      }
      if(move->get_hind()==head){
          m[tail].set_out(nullptr);
          delete move;
          return true;
      }
      if(!move->get_tlink()){
          return false;
      }
      while(move->get_tlink()->get_hind()!=head){
          move=move->get_tlink();
          if(move->get_tlink()){
              return false;
          }
      }
      temp=move->get_tlink();
      move->set_tlink(temp->get_tlink());
      delete temp;
      return true;
  }

  template<typename VertexInfo>
  inline ogdf::GraphAttributes graph<VertexInfo>::switch_graph(ogdf::Graph& g)
  {
      std::unordered_map<std::string, ogdf::node> uuidToNode;
      for (auto &[uuid, node] : this->m) {
          ogdf::node ogdfNode = g.newNode();
          uuidToNode[uuid] = ogdfNode;
      }

      for (auto &[uuid, node] : this->m) {
          arc_node *currentArc = node.get_out();
          while (currentArc != nullptr) {
              const std::string &tind = currentArc->get_tind();
              const std::string &hind = currentArc->get_hind();

              if (uuidToNode.count(tind) && uuidToNode.count(hind)) {
                  g.newEdge(uuidToNode[tind], uuidToNode[hind]);
              }
              currentArc = currentArc->get_tlink();  // 遍历下一条出边
          }
      }

      ogdf::GraphAttributes ga(g,
                               ogdf::GraphAttributes::nodeGraphics |
                                   ogdf::GraphAttributes::nodeLabel |
                                   ogdf::GraphAttributes::nodeStyle |
                                   ogdf::GraphAttributes::edgeGraphics);

      for(auto &[uuid,node]:uuidToNode){
          ga.width(node)=60;
          ga.height(node)=60;
          ga.label(node)=uuid;
      }
      return ga;
  }



} // namespace AOV

#endif
