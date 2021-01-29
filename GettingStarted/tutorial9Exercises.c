WBigTitle(Exercises)

A very large class of practically useful programs can be obtained
just by declaring
basic classes, collections
and simple Data classes.

Let's see some exercises and solutions 
to understand better how 42 code looks like

WTitle((1/5) Max method)
Write a static method Wcode(MaxOfList) returning the max from a list of numbers
WP
Basic Solution: 
OBCode
MaxOfList = {//static method pattern
  UndefinedOnEmpty = Message:{[Message.Guard]}
  //Max is undefined on empty lists.
  //Since there was no mention of preconditions, we should explicitly handle all the error cases as Guards
  class method
  Num (Num.List that) = {
    if that.isEmpty() ( error UndefinedOnEmpty"Max is undefined on empty lists" )
    //now that we know we can proceed: 
    var Num maxCandidate= that.left()
    //there is no such thing like a minimum Num, we need to select one element from the list.
    for n in that (
      //we could write 'that.withoutLeft()' to avoid checking on the first again
      if maxCandidate<n (maxCandidate:= n)
      //update the variable to keep track of the max so far.
      )
    return maxCandidate
    }
  }
CCode

Solution using Wcode(.reduce()):
OBCode
MaxOfList = {
  class method
  Num (Num.List that) =
    that.reduce()(
      for e in \vals() (
        if \acc < e \add(e) 
        )
      )
  }
CCode
Where the method Wcode(reduce())  will already throw a meaningful error in case of an empty list: Wcode(Collection.OperationUndefinedOnEmpty).
Defining your own error may still produce more readable errors, so feel free to mix and match the two approaches as show in the next exercise:


WTitle((2/5) Merge two lists of strings)
Write a static method Wcode(MapText) producing a string from to lists of strings of the same length.
 For example
Wcode(`MapText(keys=S.List[S"a";S"b";S"c"] vals=S.List[S"z";S"y";S"z"])')
should produce Wcode(`S"[a->z, b->y, c->z]"')
WP
Solution: 
OBCode
MapText = {
  UnequalSize = Message:{[Message.Guard]}
  class method
  S (S.List keys, S.List vals) = {
    if keys.size() !=  vals.size() error UnequalSize
      "keys= %keys.size(), values= %vals.size()" 
    //the former formatting allows us to keep a whole line for the error message
    res = S.List()(for k in keys, v in vals \add(S"%k->%v"))
    if res.isEmpty() return S"[]"
    text = res.reduce()(for s in \vals \add(\acc++S", %s"))
    return S"[%text]"
    }
  }
CCode
Note how we write Wcode(`\add(\acc++S", %s")')
instead of Wcode(`\add(S"%\acc, %s")') since 
 string interpolation does not currently support the Wcode(`\').

WTitle((3/5) Filtering)
Write a static method Wcode(`FilterUpTo{ class method S.List(S.List that, I size)}') filtering out from a list of strings the ones longer
than \Q@size@.
For example 
Wcode(`FilterUpTo(S.List[S"a";S"ab";S"abc"] size=2I)==S.List[S"a";S"ab"]')
WP
Precondition: Wcode(size) is not negative
WP
Solution: 
OBCode
FilterUpTo = {
  class method
  S.List (S.List that, I size) = (
    X.Pre[size >= 0I]
    S.List()(for s in that if s.size()<= size \add(s))
    )
  }
CCode
Again we see yet another way to handle errors; preconditions are appropriate when it is an observed  bug if the user calls it with wrong parameters.

WTitle((4/5) Random mole)
For a longer example, represent a piece of land as a 80*80 bi-dimensional map,
where every cell can be full of dirt (90%) or rock (10%).
Then a mole start from the left top corner and attempts to
digs through dirt randomly.
After 3000 steps the mole stops.
Define the opportune classes and write a Wcode(randomDig)
method. 
WP
To start, we define some auxiliary classes: 
OBCode
Point = Data:{I x, I y
  @Cache.Now class method
  Void invariant(I x, I y) = X.Guarded[
    x>=0I; x<80I;
    y>=0I; y<80I;
    ]

  method
  This go(Direction that) = {
    return that.go(this)
    catch error X.Guarded _ return this
    }
    
  method
  I index() = 
    (this.y()*80I)+this.x()
  }
CCode
Wcode(Point) has an invariant ensuring that the coordinates are inside the 80*80 area.
We use Wcode(X.Guarded) instead of Wcode(X) since Wcode(X.Guarded) implements Wcode(Guard), thus we can rely on such error to trigger predictably.
We do this in method Wcode(go(that)), where we capture the invariant failure in case moving the point would push it outside of the boundary; in that case we keep the point in the original coordinates.

OBCode
Direction = Collection.Enum:{interface
  method Point go(Point that)
  Up={[This1] method go(that)=that.with(x=\x-1I)}
  Down={[This1]method go(that)=that.with(x=\x+1I)}
  Left={[This1]method go(that)=that.with(y=\y-1I)}
  Right={[This1]method go(that)=that.with(y=\y+1I)}
  }
CCode
Wcode(Direction) is an enumeration, and we leverage on dyanamic dispatch to encode the beaviour of the Wcode(go(that)) method.
Note how in Wcode(Direction) we explicitly declared the top level Wcode(interface) and
we implemented the outer level Wcode(This1) explicitly in all the cases.
Then we could implemented the Wcode(go(that)) method without repeating the type signatore.

OBCode
Cell = Collection.Enum:{
  method S symbol()
  Dirt={method S symbol()=S"#"}
  Rock={method S symbol()=S.percent()}
  Empty={method S symbol()=S" "}
  Mole={method S symbol()=S"M"}
  }
CCode
Instead, while declaring Wcode(Cell) we omit the Wcode(interface) keyword and the explicit implementation.
Then we had to repeat the type signature all the times while implementing the method Wcode(symbol()).
Both ways to declare enumerations works and produce the same result.

OBCode  
Land = Data:{[HasToS]
  mut Math.Random rand
  mut Cell.List cells
  
  class method
  mut Cell.List #default#cells(mut Math.Random rand) = Cell.List[](
    for i in Range(80I*80I) (
      if rand.nextInRange(0I to=10I)==0I \add(Cell.Rock())
      else \add(Cell.Dirt())
      )
    )

  mut method 
  Void set(Point that, Cell val) =
    this.#cells().set(that.index() val=val)
    
  read method 
  Cell val(Point that) =
    this.cells().val(that.index())    
  
  mut method
  Void randomDig() = (
    var current = Point(x=0I y=0I)
    for i in Range(3000I) (
      this.set(current,val=Cell.Empty())
      d = Direction.Vals().val(this.#rand().nextInRange(0I to=4I))
      newPoint = current.go(d) //no digging in rock
      if this.val(newPoint)!=Cell.Rock() ( current:=newPoint )
      )
    this.set(current,val=Cell.Mole()) //finally, the mole is where we ends up
    )
  
  method toS() = (
    var res=S""
    for x in Range(80I) (
      res++=S.nl()//newline
      for y in Range(80I) res++=this.val(\(x=x,y=y)).symbol()
      )
    res++S.nl()
    )
  //since we define 'toS()' explicitly, Data will leave it alone :)
  }
CCode
Wcode(Land) has a Wcode(mut Math.Random rand) field and a 
Wcode(mut Cell.List cells) field; a list of cells of size 80*80.
While we expect the user to provide the random object, we wish to provide a way to initialize the 
Wcode(cells).
In 42, as for most languages, we could provide a default value for a field by writing
Wcode(mut Cell.List cells = Cell.List[](..)), but in this case we need to use the provided random object
to complete the initialization, thus we use a Wcode(mut Cell.List #default#cells(mut Math.Random rand))
instead.
As with Wcode(Cache.Now),
we take in input parameters with the same name of the fields we wish to use.
Wcode(Data) is going to rely on this method to initialize the Wcode(cells) field.
This is actually a general pattern of Wcode(Data), allowing default values for any method.
WBR
The method Wcode(randomDig()) is straightforward.
WBR
Note how we override Wcode(toS()) instead of accepting the implementation provided by Wcode(Data).

To use the Wcode(Land) class we can use the code below.
OBCode
MainMole = (
  land = Land(rand=\.#$random())
  land.randomDig()
  Debug(land)
  )
CCode


WTitle((5/5) Examples summary)
<ul><li>
Always think about what can go wrong upfront
</li><li>
Many methods can be completed by first checking for 
errors/issues and then using a Wcode(for), possibly inside a Wcode(reduce()) or a Wcode(Match).
</li><li>
Before heading into a problem,
spend some time to define your problem domain.
We dodged a lot of headaches by defining
points with invariants.
</li></ul>