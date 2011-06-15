use bigint;

$first = 1;
$second = 1;
$str = '';
$count = 2;
while(1){
	$count++;
	$temp = $first;
	$first += $second;
	$second = $temp;
	#print $count ." ".$first."\n";
	$str = sprintf("%s",$first);
	if(length $str >= 1000){
		print $count;
		exit();
	}

}