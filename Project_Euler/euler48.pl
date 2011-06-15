use bigint;
$sum =0;
for($i=1;$i<=1000;$i++){
	$sum += $i ** $i;
}
print $sum."\n";