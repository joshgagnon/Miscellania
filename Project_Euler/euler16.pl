use bigint;
$big =2**1000;
$res = sprintf("%s",$big);
@nums = split //, $res;
$sum = 0;
foreach $num (@nums){
	$sum+=$num;
}
print $sum;