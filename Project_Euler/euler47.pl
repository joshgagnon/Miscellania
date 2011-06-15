use strict;
use warnings;



sub sievefactors{
  my $max = $_[0];
  my @erato;
  my @primes = ();
  my $sqr = sqrt($max); 
  my $count =0;
  for(my $j=2;$j<$sqr;$j++){ 
  	if (not defined @{$erato[$j]}){
  		for(my $i=$j*2; $i<$max;$i=$i+$j){ 
  			my $k = $i;
  			my $m = 1;
  			while($k % $j == 0){
				$m *= $j;
  				$k /= $j;
  			}
  			push @{$erato[$i]}, $m;
  		} 	
  	}
  } 	
  return @erato;
}

my @factors = sievefactors(1000000);
my ($i,$j);
    for $i ( 0 .. $#factors ) {
    	if (defined @{$factors[$i]} && $#{$factors[$i]} == 3 && 
    		defined @{$factors[$i+1]} && $#{$factors[$i+1]} == 3 && 
    		defined @{$factors[$i+2]} && $#{$factors[$i+2]} == 3 && 
    		defined @{$factors[$i+3]} && $#{$factors[$i+3]} == 3    	
    	){
        	print "\t elt $i is [ @{$factors[$i]} ],\n";
        	print "\t elt ".($i+2)." is [ @{$factors[$i+1]} ],\n";
        	print "\t elt ".($i+3)." is [ @{$factors[$i+2]} ],\n";
        	print "\t elt ".($i+4)." is [ @{$factors[$i+3]} ],\n\n";
        	my %dict;
        	my $win = 1;
        	for($j=$i;$j<=$i+4;$j++){
        		foreach my $fact (@{$factors[$j]}){
        			if(defined $dict{$fact}){
        				$win = 0;
        			}
        			else{
        				$dict{$fact} =1;
        			}
        		}      	
        	}
        	if($win==1){
        		exit();
        	}
        }
 }
    
#some good code down in the comments (by good, i mean useful tricks)    
=pod
sub sievefactors{
  my $max = $_[0];
  my @erato;
  my @primes = ();
  my $sqr = sqrt($max); 
  my $count =0;
  for(my $j=2;$j<$sqr;$j++){ 
  	if (not defined @{$erato[$j]}){
  		for(my $i=$j*2; $i<=$max+1;$i=$i+$j){ 
  			my $k = $i;
  			while($k % $j == 0){
  				push @{$erato[$i]}, $j;
  				$k /= $j;
  			}
  		} 	
  	}
  } 	
  return @erato;
}

my @factors = sievefactors(100);
my $i;
    for $i ( 0 .. $#factors ) {
    	if (defined @{$factors[$i]} ){
        print "\t elt $i is [ @{$factors[$i]} ],\n";
        }
    }
=cut

=pod
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
  	for(my $i=$j*$j; $i<=$max+1;$i=$i+$j){ 
  		$erato[$i]=0;
  		}
  	}
  for(my $i=0;$i<$max;$i++){ if($erato[$i]==1){ $count++;}}
  printf("Primes: %d\n", $count);
  for(my $j=0;$j<$max;$j++){ if($erato[$j] == 1){ push @primes, $j;}}
  return (\@erato, \@primes);
}

sub factors{
	my ($num,@list) = @_;
	my $i = 0;
	my %dict;
	while($num > 1){
		if($num % $list[$i] == 0 ){
			$num /= $list[$i];
			$dict{$list[$i]}++;
		}
		else{
			$i++;
		}
	}
	my @array;
	while ( my ($key, $value) = each(%dict)) {
		push @array, $key*$value;
		print  $key." ".$value ."\n";
		
	}
	print "length is ".(scalar(@array));
	print "\n";
	return @array;
}

sub unique{
	my (@list) = shift;
	my %test;
	my ($i, $j);
	for $i (0 .. $#list){
		for $j ( 0 .. $#{$list[$i]} ) {
			print $list[$i][$j]."\n";
			if (defined $test{$list[$i][$j]}){
				return 0;
			}
			$test{$list[$i][$j]} = 1;
		}				
	}
	return 1;
}


sub main{
	my ($erato_ref,$primes_ref) = sieve(100000);
	my @erato = @$erato_ref;
	my @primes = @$primes_ref;
	my @listofacts;
	for(my $j=100;$j<100000;$j++){
		#print "$j  $erato[$j]\n";
		if(not $erato[$j]){
			print $j."\n";
			push @listofacts, [factors($j,@primes)];
			#now test for uniqueness
			if($j>104 && $#{$listofacts[-1]} == 3 && $#{$listofacts[-2]} == 3 
				&& $#{$listofacts[-3]} == 3 && $#{$listofacts[-4]} == 3){
				
				print "in here\n";
				exit();
			}			
		}
		else{
			push @listofacts, [0];
		}
	}
}

main();
=cut
