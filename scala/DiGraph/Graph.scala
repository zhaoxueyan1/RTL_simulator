package DiGraph

import java.io.{File, PrintWriter}

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
        val writer = new PrintWriter(new File("./Output.txt" ))
        writer.println(NodeMP.size)
        NodeMP.foreach((e)=>{
            val t = e._2
            writer.write(s"${t.id} ${t.name} ${t.tpe} ${t.cppType} ${t.cppWidth} ${t.dir}  ${t.expr}\n")
        })
        writer.println(EdgeSet.size)
        EdgeSet.foreach(e=>writer.write(MP(e.from)+" "+MP(e.to)+"\n"))
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