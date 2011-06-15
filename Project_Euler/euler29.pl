use bigint;
%happy;


foreach $i (2..100){ 
	foreach $j (2..100){
		$k = sprintf("%.f", $i**$j);	# the 'trick'
		 $happy{$k}=1;
	}
}
$count=0;
while(($key,$value) = each (%happy)){
	#print $key." ".$value."\n";
	$count++;
}
print ($count)."\n";