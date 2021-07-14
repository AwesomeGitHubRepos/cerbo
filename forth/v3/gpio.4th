\ doesn't work
variable pins
: `on " gpio-set " type . cr ;
: on dup postpone literal pins ! postpone `on  ; immediate
: s 12 on ;

