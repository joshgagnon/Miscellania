sub gcm{
    ($a,$b) = @_;
    return ($b == 0) ? ($a):( gcm($b, $a % $b ) );
}

sub lcm{
    ($a,$b) = @_;
    return ( $a * $b ) / gcm($a,$b) ;
}

$result = lcm(2,3);
for($i=4;$i<=20;$i++){
    $result = lcm($i,$result);
}
print $result;
