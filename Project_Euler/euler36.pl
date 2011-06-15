sub dec2bin {
    my $str = unpack("B32", pack("N", shift));
    $str =~ s/^0+(?=\d)//;   # otherwise you'll get leading zeros
    return $str;
}
sub bin2dec {
    return unpack("N", pack("B32", substr("0" x 32 . shift, -32)));
}


sub isPalin{
	$str =shift;
	if (length($str) == 1 or length($str) == 0) { return 1;}
	if (int(substr $str, 0, 1) != int( substr $str, -1, 1) ){;return 0;}
	return isPalin(substr $str, 1, length($str) -2);
}	

$count = 0;
for($i=1;$i<1000000;$i++){
	if (isPalin($i) and isPalin(dec2bin($i))){
		$count += $i;
		print "$i" . " ". dec2bin($i)."\n";
	}

}
print $count."\n";
