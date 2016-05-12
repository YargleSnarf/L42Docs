WBigTitle(Basics)
WTitle((1/5)Simple hello world program)
Let's look at a simple hello world program:
OBCode 
{reuse L42.is/AdamTowel
Main:{
  Debug(S"Hello world")
  return ExitCode.success()
  }
}
CCode
At the right of Wcode(Main:) we write the expression that
we wish to execute; in this case a sequence of two statements.
Wcode(Main) is not a method, and Wcode(Main) is not special name either. In 42 there is no concept of main method as in
Java or C. For now you can think of Wcode(Main:) as a top level command, we will understand later how this fits with the general language design.

When we write Wcode(reuse L42.is/AdamTowel) we are asking 42 to
reuse the code offered by the library offered in the internet address 
Wcode(L42.is/AdamTowel).
AdamTowel is our Towel, that is the set of classes and interfaces that we wish to start from (Wlink(towel,see more on towels)).
Wcode(L42.is) is the main website of 42, where most commonly used libraries are hosted. To reuse code you 
need an internet connection; but this also means that you will never have to manually import any code. Required code will be transitively downloaded and cached on your machine, so you need not to be aware of the existence of this mechanism.
WP
We do not need to always start from AdamTowel, there are many interesting towels out there, and you may also become skilled in the 
advanced technique of towel embroidery.
WP

WTitle((2/5)Method declaration and call)
Let's now declare a method and call it.
OBCode
{reuse L42.is/AdamTowel
MyCode:{
  class method
  S hello(S name){
    return S"hello "++name
    } 
  }
Main:{
  Debug(MyCode.hello(name:S"world"))
  return ExitCode.success()
  }
}
CCode
Here we declare a class to host our Wcode(hello(name)) method.
We write Wcode(class method) to declare a method that can be called on the class object, as in Wcode(MyCode.hello(name:S"world")). This is roughly equivalent to a static method in languages like Java or C++, or class methods in Python.

WP
Note how the method is called using the parameter name explicitly.
We believe this increases readability.
WP
You can also notice how there are two different usages for curly brackets: if there is at least a return keyword it is a set of statements,
otherwise is a library literal, and can contains methods and nesteds.
A nested (class or interface) is denoted by an upper-case name, and can be obtained by a library literal or an expression producing a library literal.
A library can be a class (default case) or an interface (starts with the Wcode(interface) keyword).
A nested in 42 is similar to a static inner class in Java, or a nested class in C++. It is just a convenient way to divide in a tree shape the various components of our program.
WP
The class Wcode(MyCode) from before offers a single class method, has no fields and you can not create instances of Wcode(MyCode), since no factory is present; you can think about it as a module.
In 42 we do not have constructors. Objects are created by factory methods. We believe this is a much simpler and more consistent approach to object initialization.

WTitle((3/5)Simple class with internal state)
Let's create now a class with state and factory:
OBCode
Point:Data<<{
  Num x
  Num y
  method
  Point add(Num x)
    Point(x:x+this.x(), y:this.y())
  Point add(Num y)
    this.with(y:y+this.y())
  }
CCode
Here you can see we declare a Wcode(Point) class with coordinates Wcode(x) and Wcode(y) of type Wcode(Num),
 unlimited precision rational number.
Wcode(Data) is a decorator. Decorators are classes/objects that offer an operator Wcode(<<), called the babel fish operator,
whose goal is to translate a library into a "better" library.
In this case, Wcode(Data) is translating the class Wcode(`{Num x, Num y}') into a much longer class, with
a factory method taking in input the fields and initializing them; but also containing
 boring but useful definitions for
equality, inequality, conversions from and to human readable strings, XML and binary representations for (de)serialization.
WP
Finally, we declare a method to add to each of the coordinates.
For very short methods we can omit the curly brackets and Wcode(return).
Indeed, method bodies are just expressions, and the curly brackets turn a block of statements into one expression. 

In the method Wcode(add(x)) we show a constructor call and getters.
In the method Wcode(add(y)) we show an improved version, using the Wcode(with) method, another gift of Data, that allows us to easily create a clone with 
one or more fields updated.
We can declare two methods, Wcode(add(x)) and Wcode(add(y)) with the same name, if parameter names are different.
WP
Note how we always use getters and we never access fields directly.
In many other languages we can use write Wcode(a.fieldName) and Wcode(a.fieldName=newValue). Such syntax does not exists in 42. The same goes for object instantiation; in many languages there is a special Wcode(new ClassName(..)) dedicated syntax, while in 42 it is just a method call.
WP
Also, similarly to what happens in Python, we need to use Wcode(this.methodName()) to call methods when the receiver is Wcode(this).
While this makes some code more verbose, it saves us from the burden of  method scope-hiding; that
is, we do not need to worry about scoping and nesting for method resolution.


WTitle(Decorators)
Decorators are one of the main concepts used by 42 programmers. We will encounter many decorators in this tutorial.
For now, just get used to the pattern of writing
Wcode(<<) to go from a minimal chunk of code, with method declarations for the important bits, to a fully fledged usable class.
Wlink(Decorators, More on decorators)


WTitle((4/5)Vectors)

Vectors can be declared using Wcode(Collections.vector(of)), as in the example below.

OBCode
Nums:Collections.vector(of:Num) //declaration for vectors of nums
Points:Collections.vector(of:Point) //same for points
/*..*/
xs=Nums[10Num;20Num;30Num]
ys=Nums[1Num;2Num;3Num]
points=Points[with x in xs.vals(), y in ys.vals() (
  use[Point(x:x, y:y)]
  )]
//points=Points[ Point(x:10Num, y:1Num);
// Point(x:20Num, y:2Num); Point(x:30Num, y:3Num)]
CCode

Vectors can be initialized with the Wcode([_;_;_])
syntax or with the Wcode([with _ ( _ use[ _ ]_ )]) syntax, allowing to build
a collection by committing values obtained during an iteration.

We will discuss all the variation of Wcode(with) later.
Note that to express numeric literals we need to specify their class.

WTitle(`(5/5)Basis summary')

<ul><li>
At the start of your program, import a towel using 
"Wcode(reuse _)", as in Wcode(reuse L42.is/AdamTowel).
</li><li>
To declare a simple class exposing its state and 
some methods working with those, use Wcode(Data), as in
Wcode(`Point:Data<<{Num x, Num y}').
</li><li>
You can declare methods in classes with the Wcode(method) keyword,
as in Wcode(method ReturnType myName(ParType parName) body).
Use Wcode(class method) for methods that can be called on the class object directly.
</li><li>
To define a vector of a type, use 
Wcode(Collections.vector(of))
as in 
Wcode(Points:Collections.vector(of:Point))
</li></ul>


WTitle(`Object creation summary')
42 supports many different syntactic forms that are convenient for creating objects:
<ul><li>
12Num: from a numeric representation
</li><li>
S"foo": from a string representation
</li><li>
Point(x:_,y:_): from the parameter values
</li><li>
Points[_;_;_]: from a variable length sequence of values.
</li></ul>

Note that in 42 those are all just expressions, and represent methods in the named class.
Sometimes it is convenient to reuse this kind of syntax to get better syntactic support for certain operations; for example, as we will see later, the string class uses square brackets to support string formatting.
Wlink(S, To understand it better: a guide to the S class)
