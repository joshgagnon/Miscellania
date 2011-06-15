$first = 1;
$second = 2;
$sum = 0;
while($second < 4000000){
    if($second % 2 == 0){
	$sum += $second;
    }
    $temp = $second;
    $second = $second + $first;
    $first = $temp;
    
    print $second."\n";
}

print $sum."\n";
