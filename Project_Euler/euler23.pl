sub sum_divisors{
	($num) = @_;
	$sum=0;
	for($i=1;$i<=sqrt($num);$i++){	
		if($i == sqrt($num)){
			$sum += $i;
		}
		elsif ($num % $i == 0){
			$sum += $i;
			$sum += $num/$i;
		}
	}	
	return $sum - $num;
}
@abundants;
$max = 28123;
for($j=1;$j<=28124;$j++){
	if ($j < sum_divisors($j)){
		#print " ".$j."\n";
		push @abundants, $j;
	}
}

@ints = (0...$max-1);
for($i=0;$i<=$#abundants;$i++){
	for($j=$i;$j<=$#abundants;$j++){
		if($abundants[$i] + $abundants[$j] < $max){
			$ints[$abundants[$i] + $abundants[$j]] = 0;
		}
	}
}
$sum = 0;
foreach $int (@ints){
	print $int." ";
	$sum += $int;
}
print $sum;