use strict;
#doing it the dump way
my $str = ".";
for(my $i=1;$i<=1000000;$i++){
	$str .= sprintf("%d",$i);
}
my @array = split '',$str;
print ($array[100] * $array[1000] * $array[10000] *$array[100000] *$array[1000000] )."\n";