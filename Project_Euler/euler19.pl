my @months = (31,28,31,30,31,30,31,31,30,31,30,31);

#sunday is any day that % 7 == 0

$day = 1;
$month = 1;
$year = 1901;
my $sundays = 0;
my @days;
my $pos = 0;
while($year < 2001){
	#print $day." / ".$month." / ".$year."\n";
	if($day == 1 and $pos % 7 ==0){
		$sundays++;
	}
	@days[$pos++] = $day++;
	if($day > $months[$month-1]){
		$day = 1;
		$month++;
		if($month > 12){
			$month = 1;
			$year++;
			if(($year%4==0 and not $year%100==0) or $year%400==0){
				$months[1]=29;
			}
			else{
				$months[1]=28;
			}
		}
	}
}
print $sundays."\n";