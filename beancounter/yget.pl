  use Finance::QuoteHist::Yahoo;
$q = new Finance::QuoteHist::Yahoo->new
#$q = getquote

     (
      symbols    => [qw(VOD.L)],
      start_date => '6 days ago',
      end_date   => 'today',
     );

#print join("\n", @q);
#print "@q\n";
#exit 0;
  # Values
#  foreach $row (@q->quotes()) {
foreach $row (@q) {
    ($symbol, $date, $open, $high, $low, $close, $volume) = @$row;
    #print @$row;
    print join(" ", @$row), "\n";
  }
