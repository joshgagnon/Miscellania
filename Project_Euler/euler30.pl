use strict;

sub getPowerSum{
	my ($num, $pow) = @_;
	my @str = split '', sprintf("%d",$num);
	my $sum = 0;
	for my $dig (@str){
		$sum+= $dig**$pow;
	}
	return $sum;
}
my $result = 0;
for(my $i=2;$i<1000000;$i++){
	if(getPowerSum($i,5) == $i){
		$result += $i;
	}
}
print $result."\n";