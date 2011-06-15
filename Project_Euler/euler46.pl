use strict;

sub sieve{
  my $max = $_[0];
  my @erato;
  my @primes = ();
  my $sqr = sqrt($max); 
  my $count =0;
  $erato[0]=0;
  $erato[1]=0;
  for(my $i=2;$i<$max;$i++){ $erato[$i]=1;}
  for(my $j=2;$j<$sqr;$j++){ 
  	for(my $i=$j*$j; $i<$max;$i=$i+$j){ 
  		$erato[$i]=0;
  		}
  	}
  for(my $i=0;$i<$max;$i++){ if($erato[$i]==1){ $count++;}}
  printf("Primes: %d\n", $count);
  for(my $j=0;$j<$max;$j++){ if($erato[$j] == 1){ push @primes, $j;}}
  return (\@erato, \@primes);
}


my ($erato_ref, $primes_ref) = sieve(1000000);
my @erato = @$erato_ref;
my @primes = @$primes_ref;

for( my $i=9;$i<1000000;$i+=2){
	my $pass = 0;
	if(not $erato[$i]){
	for(my $x=0;$primes[$x]<$i;$x++){
		for(my $y = 1; $y * $y < $i; $y++){
			if($primes[$x] + 2*($y*$y) == $i){
				$pass =1;
			}
		}
	}
	if($pass ==0){
		print $i."\n";
		exit();
	}
	}
}