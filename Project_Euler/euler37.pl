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

sub truncatable{
	my ($num,@erato) = @_;
	$erato[0]=1;
	return 1;
}



my @erato ;

sub main{
	my $max =5000000;
	my $sum=0;
	my ($erato_ref, $primes_ref) = sieve($max);
	my @primes = @{$primes_ref};
	my @erato = @{$erato_ref};
	SLAM: foreach my $prime (@primes){
		if(length $prime > 1){
		for(my $i=1;$i < length $prime; $i++){
			#truncatable(1,@erato);
			if(not $erato[substr($prime,$i)] or not $erato[substr($prime,0,$i)]){
				next SLAM;
			}
		}
			print $prime."\n";
			$sum += $prime;
		#print "\n";
	}
	}
	print "\nSum = ".$sum."\n";
	truncatable(3797,$erato_ref);
}


main();
