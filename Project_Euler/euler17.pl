@ones = qw(null one two three four five six seven eight nine);
@teens = qw(ten eleven twelve thirteen fourteen fifteen sixteen seventeen eighteen nineteen);
@tens = qw(null null twenty thirty forty fifty sixty seventy eighty ninety);
$and = ' and ';
$hundred = ' hundred';
$thousand = ' thousand '; 
$str='';
for($i=1;$i<1000;$i++){
	if(($i % 1000)/100 != 0 and $i > 99){
		print $ones[($i % 1000)/100].$hundred;
		#$str .=  $ones[($i % 1000)/100].$hundred;
	}
	if($i % 100 != 0 and $i > 100) { 
		print $and; 
		#$str .= $and;
	}
	if(($i % 100)/10 != 0 and ($i % 100)/10 != 1 and ($i % 100) > 19){
		print $tens[($i % 100)/10]." ";
		#$str .=  $tens[($i % 100)/10];
	}
	if($i%100 > 9 and $i%100 < 20){
		print $teens[$i%100 - 10];
		#$str .= $teens[$i%100 - 10];
	}
	elsif($i % 10 != 0){
		print $ones[$i % 10];
		#$str += $ones[$i % 10];
	}

	print "\n";
}
print "one thousand\n";
#$str .= "onethousand";
print $str;