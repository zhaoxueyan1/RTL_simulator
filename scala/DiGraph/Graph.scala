package DiGraph

import firrtl.ir.DefModule

import scala.collection.mutable

class Graph {
    // name -> Node
    var NodeMP = mutable.Map[String, Node]()
    // edge collection
    var EdgeSet = mutable.Seq[Edge]()
    // the name->id
    var MP = mutable.Map[String,Int]()
    // the number of Node
    var cnt = 0
    var moduleMap = mutable.Map[String,DefModule]()

    def Output()={
        NodeMP.foreach((e)=>{
            val t = e._2
            printf("%s %s %s %s %s %s\n",t.id,t.tpe,t.cppType,t.dir,t.name,t.expr)
        })
        EdgeSet.foreach(e=>println(MP(e.from)+" "+MP(e.to)))
    }

    def addNode(name:String, cppType:String,cppWidth:BigInt, dir:String,tpe:String, expr:String):Node={
        MP += (name -> cnt)
        val node = new Node(name, s"${cnt}", cppType, cppWidth, dir, tpe, expr)
        NodeMP += (name -> node)
        cnt += 1
        node
    }

    def serialize: String = {
        return  "\n"
    }

}