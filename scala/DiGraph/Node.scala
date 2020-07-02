package DiGraph
import scala.math.BigInt;

/*
 * name: the name of this signal
 * id: the index of MP for String
 * cppType: UInt, SInt, Float, ...
 * cppWidth: len
 * dir: If is a Port
 * typ: Reg wire Mux Validif Node
 * ext: dynamic  i.e. Uint : 10
 * */
class Node(var name:String,
           var id:String,
           var cppType:String,
           var cppWidth:BigInt,
           var dir:String,
           var tpe:String,
           var expr:String
          )
/*
* from :
* to  :
* */
class Edge(var from:String,
           var to:String,
          )