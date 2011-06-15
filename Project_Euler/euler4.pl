

sub isPalin{
	$str =shift;
	if (length($str) == 1 or length($str) == 0) { return 1;}
	if (int(substr $str, 0, 1) != int( substr $str, -1, 1) ){;return 0;}
	return isPalin(substr $str, 1, length($str) -2);
}	
	
$max = 0;

for($i=100;$i<=999;$i++){
	for($j=100;$j<=999;$j++){
		
		$st = sprintf("%d",$i*$j);
		if( isPalin($st) ){
			if($i*$j > $max){ $max = $i*$j;	}		
		}
	}
}

print $max."\n";