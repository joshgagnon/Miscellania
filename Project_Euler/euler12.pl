


sub num_divisors{
	($num) = @_;
	$count=0;
	for($i=1;$i<=sqrt($num);$i++){
		if ($num % $i == 0){
			$count += 2;
		}
	}
	return $count;
}

$k = 1;
for($j=2;;$j++){
	$k += $j;
	if(500 < num_divisors($k)){
		print $k."\n";
		exit();
	}
	print $k. " ".num_divisors($k)."\n";
}