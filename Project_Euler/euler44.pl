use strict;

my (@pent,%penty);

for(my $i=1;$i<1000000;$i++){
	push @pent,$i*(3*$i -1)/2;
	$penty{$i*(3*$i -1)/2}=1;
}
my $min = 100000000;
for (my $i =0;$i<= $#pent;$i++){
	for (my $j =$i+1;$j<= $i + 10000;$j++){
		#print "$pent[$i] $pent[$j]\n";
		if(defined $penty{($pent[$j]+$pent[$i])} and defined $penty{($pent[$j]-$pent[$i])} and $pent[$j]-$pent[$i] < $min){
			print "best so far: $pent[$i] $pent[$j] ".($pent[$j]-$pent[$i])."\n";
			$min = $pent[$j]-$pent[$i];
		}
	}
}