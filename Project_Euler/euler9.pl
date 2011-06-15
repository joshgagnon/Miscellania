for($i=0;$i<1000;$i++){
	for($j=$i+1;$j<1000;$j++){
		if(int(sqrt($i*$i + $j*$j)) == sqrt($i*$i + $j*$j) && (sqrt($i*$i + $j*$j) + $i + $j) == 1000){
			print $i*$j*sqrt($i*$i + $j*$j)."\n";
		}
	}
}