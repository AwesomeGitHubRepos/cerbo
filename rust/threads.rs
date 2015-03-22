//#![feature(old_io)]
use std::thread;
use std::old_io::timer;
use std::time::duration::Duration;

fn main() {
    let vin = vec![1.4f64, 1.2f64, 1.5f64];
    let mut guards = Vec::with_capacity(3);

    for i in 0..3 {
        let inval = vin[i];
        guards.push(thread::scoped( move || {
            let ms = (1000.0f64 * inval) as i64;
            let d = Duration::milliseconds(ms);
            timer::sleep(d);
            println!("Waited {}", inval);
            10.0f64 + inval
        }));
    }

    let mut answers = Vec::with_capacity(3);
    for guard in guards {
        let answer = guard.join();
        answers.push(answer);
        //println!("{}", answer);
    };

    for ans in answers { println!("{}", ans) } ;

}
