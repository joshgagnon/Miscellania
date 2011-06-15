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

sub isPandigital{
	my $num = @_[0];
	my $str = sprintf("%d",$num);
	my $len = length $str;
	my @let = sort split('',$str);
	my @slots;
	for(my $i=0;$i<$len;$i++){
		if(int($let[$i])==0){
			return 0;
		}
		$slots[int($let[$i])] = 1;
	}
	for(my $i=1;$i<$len+1;$i++){
		if(not defined $slots[$i]){
			return 0;
		}
	}
	return 1;
}




my ($erato_ref, $primes_ref) = sieve(10000000);
my @erato = @$erato_ref;
my @primes = @$primes_ref;
for my $p (@primes){
	if(isPandigital($p)){
		print $p."\n";
	}
}