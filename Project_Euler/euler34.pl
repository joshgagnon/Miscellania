use strict;
my @factorials;
$factorials[0] = 0;
$factorials[1] = 1;
for my $i (2..9){
	$factorials[$i] = $i * $factorials[$i-1];
	print $factorials[$i]."\n";
}

for my $i (10..100000000){
	my @num = split '', $i;
	my $sum = 0;
	foreach my $ber (@num){
		$sum+=$factorials[$ber];
	}
	if($sum == $i){
		print $i."\n";
	}
	if($sum > $i){
		$i+=8;
	}
}