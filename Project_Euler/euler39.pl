#first, gen all pythogorean triples

use strict;

my $max = 1000;
my @results;
my %pyth;
for(my $i=1; $i < $max; $i++){
	for(my $j = 1; $j < $max; $j++){
		my $hypo = sqrt($i*$i + $j*$j);
		if($hypo == int($hypo) ){
			#print "$i $j $hypo\n";
			if($j+$i+$hypo < $max){
				@results[$j+$i+$hypo]++;			
			}
		}
	}	
}
my $max = 0;
for(my $i=1; $i <= $#results; $i++){
	if($results[$i] > $max){
		print $i."\n";
		$max = $results[$i];
	}

}