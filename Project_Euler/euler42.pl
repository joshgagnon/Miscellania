use strict;

my %tri;

for(my $i=1;$i<1000000;$i++){
	$tri{$i*($i+1)/2} = $i;
}

my %hash;
@hash{("A".."Z")} = (1..26); 
my $sum =0;


my @names = sort split /",?"?/, <>;
for(my $i=0;$i<=$#names;$i++){
	my @letters = split //, $names[$i];
	my $word = 0;
	foreach my $letter (@letters){
		$word += $hash{$letter};		
	}
	if(defined $tri{$word}){
	$sum++;
	}
}
print $sum."\n" 