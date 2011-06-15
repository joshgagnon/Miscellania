my @coins = (1,2,5,10,20,50,100,200);
my $goal = 200;

my @path;
my $sum = 200;
my $next = 0;

for my $i (0..199){
	push @path, 0,
}

$count = 1;
while($#path >= 1){
	$sum -= $coins[pop @path];
	$next = pop @path;
	$sum -= $coins[$next];
	$next+=1;
	$sum += $coins[$next];
	push @path,$next;
	if($sum==$goal){
		$count++;
	}
	while($sum < $goal){		
		$sum += $coins[$next];		
		push @path,$next;
		if ($sum == $goal){
			$count++;
		}
	}
}

print $count."\n";
