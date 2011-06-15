sub collatz{
	($num,$count) = @_;
	if( $num == 1){
		return $count+1;
	}
	if ( $num % 2 == 0) {
		return collatz($num/2, $count+1);
	}
	else{
		return collatz($num*3 + 1, $count+1);
	}
}

$max = 0; $start;
for($i = 2;$i < 1000000; $i++){
	$res = collatz($i);
	if ($res > $max){
		print $i." ".$max."\n";
		$max = $res;
		$start = $i;
	}
}

print $i."\n";