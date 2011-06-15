sub sum_divisors{
	($num) = @_;
	$sum=0;
	for($i=1;$i<=sqrt($num);$i++){
		if ($num % $i == 0){
			$sum += $i;
			$sum += $num/$i;
		}
	}
	return $sum - $num;
}

$amicable = 0;
for($j=1;$j<10000;$j++){
	$this = sum_divisors($j);
	if($j == sum_divisors($this) and $this != $j){
		print $this. " " .$j."\n";
		$amicable += $this + $j;
	}
	#print $j. " ".num_divisors($j)."\n";
}
print ($amicable/2);
print "\n";