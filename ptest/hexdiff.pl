use strict;
use warnings;
use bigint;

# Check for filename as a command-line argument
if (!@ARGV) {
    die "Usage: perl hex_diff.pl <filename>\n";
}

my $filename = $ARGV[0];

open(my $fh, '<', $filename) or die "Cannot open file '$filename': $!";

my $prev_hex;

while (my $hex_str = <$fh>) {
    chomp($hex_str);  # Remove newline

    if (defined $prev_hex) {
        my $prev_dec = hex($prev_hex);
        my $curr_dec = hex($hex_str);
        my $diff = $curr_dec - $prev_dec;
        print "Difference: $diff\n";
    }

    $prev_hex = $hex_str;
}

close($fh);
