use strict;
sub isPandigital{
	my $num = @_[0];
	my $str = sprintf("%d",$num);
	my $len = length $str;
	my @let = sort split('',$str);
	my @slots;
	for(my $i=0;$i<$len;$i++){
		if(int($let[$i])==0){
			return 0;
		}
		$slots[int($let[$i])] = 1;
	}
	for(my $i=1;$i<$len+1;$i++){
		if(not defined $slots[$i]){
			return 0;
		}
	}
	return 1;
}


for(my $i=1;$i<100000;$i++){
	my $str ='';
	my $j = 1;
	do {
		$str .= sprintf("%d",($j*$i));
		$j++;
	} while(length $str < 9);
	#print $str."\n";
	if(isPandigital(int($str))){
		print $i." ".$str."\n";
	}
}