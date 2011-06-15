use strict;

my (%tri, %pent, %hex);

for(my $i=1;$i<1000000;$i++){
	$tri{$i*($i+1)/2} = $i;
	$pent{$i*(3*$i -1)/2} =$i;
	$hex{$i*(2*$i-1)} =$i;
}
for my $key (keys %tri){
	if(defined $pent{$key} and defined $hex{$key}){
	
		print $key."\n";
		}
}