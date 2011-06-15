@a = (1..100);
@b = ();
$suma = eval join '+', @a;
for($i=1;$i<=100;$i++){
	push @b,$i*$i;
}
$sumb = eval join '+', @b;
print $sumb."\n";
print $suma*$suma."\n";
print  ((int($suma)*int($suma)) - int($sumb));
print "\n";