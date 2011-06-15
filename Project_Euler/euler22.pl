$sum =0;
@hash{("A".."Z")} = (1..26); 

@names = sort split /",?"?/, <>;
for($i=0;$i<=$#names;$i++){
	@letters = split //, $names[$i];
	$word = 0;
	foreach $letter (@letters){
		#print "$letter = $hash{$letter}\n";
		$word += $hash{$letter};		
	}
	$sum += $word * ($i);
}
print $sum."\n" 