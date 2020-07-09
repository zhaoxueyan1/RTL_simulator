package DiGraph

import java.io.{File, PrintWriter}

import firrtl.ir.DefModule

import scala.collection.mutable

class Graph {
    // instanceID,name -> Node
    var NodeMP = mutable.Map[(Int,String), Node]()
    // edge collection
    var EdgeSet = mutable.Seq[Edge]()
    // the instanceId,name->id
    var MP = mutable.Map[(Int,String),Int]() //
    // the Instance id -> (Instance name ,module name)
    var InstanceNode = mutable.Seq[(String,String)]()
    // id -> fa's id
    var InstanceGraph = mutable.Map[Int,Int]()
    // the number of Node
    var InstanceLinkMP = mutable.Map[(Int,String),Int]()
    var cnt = 0
    var instanceCnt = 0
    var moduleMap = mutable.Map[String,DefModule]()

    def Output()={
        val writer = new PrintWriter(new File("./Output.out" ))
        writer.println(InstanceNode.size)
        var i = 0
        InstanceNode.foreach(e=>{
            writer.println(i+" "+e._1+" "+e._2)
            i+=1
        })
        writer.println(InstanceGraph.size)
        InstanceGraph.foreach(e=>{
            writer.println(e._1+" "+e._2)
        })
        writer.println(NodeMP.size)
        NodeMP.foreach((e)=>{
            val t = e._2
            writer.write(s"${t.id} ${t.instanceID} ${t.name} ${t.tpe} ${t.cppType} ${t.cppWidth} ${t.dir}  ${t.expr}\n")
        })
        writer.println(EdgeSet.size)
        EdgeSet.foreach(e=>writer.write(MP(e.from)+" "+MP(e.to)+"\n"))
        writer.close()
    }

    def addNode(InstanceID:Int, name:String, cppType:String,cppWidth:BigInt, dir:String,tpe:String, expr:String):Node={
        MP += ((InstanceID,name) -> cnt)
        val node = new Node(InstanceID, name, s"${cnt}", cppType, cppWidth, dir, tpe, expr)
        NodeMP += ((InstanceID,name) -> node)
        cnt += 1
        node
    }

    def serialize: String = {
        return  "\n"
    }

}