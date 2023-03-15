/* 
  This is a mpl program with correct syntax but bad typing.
  Running this should produce type errors.
*/

x := 1;


var x : int := "8"; // int can't be initialized to string
var y : string := 5 + "hello"; // Can't add string to int

var z : bool := 1 = 1; // This is correct

for z in "2"..x do 


end for;

if y do end if;

print x;
print "\n";
print y;
print "\n";
print (!z) * z; // can't multiply booleans

print (("?" + 1) + 1) + 1;
