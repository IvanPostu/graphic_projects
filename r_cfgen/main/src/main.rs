use std::{
    env,
    fs::OpenOptions,
    io::{BufWriter, Write},
};
use utils::get_num_5;

const INVALID_ARGS_ERR_MESSAGE: &str = "Expected cli argument: -outfile=<out_file_path> <args>";

fn main() -> std::io::Result<()> {
    #[allow(unused_variables)]
    let num = get_num_5();
    let args: Vec<String> = env::args().collect();
    let mut outfile: Option<String> = None;

    if args.len() < 3 {
        panic!("{}", INVALID_ARGS_ERR_MESSAGE)
    }

    if let Some(value) = args.get(1).and_then(|arg| arg.strip_prefix("-outfile=")) {
        outfile = Some(value.to_string());
    }

    let outfile_name = match outfile {
        Some(value) => value,
        None => panic!("{}", INVALID_ARGS_ERR_MESSAGE),
    };

    let file = OpenOptions::new()
        .write(true)
        .truncate(true)
        .create(true)
        .open(outfile_name)?;
    let mut writer = BufWriter::new(file);
    for arg in args.iter().skip(2) {
        writeln!(writer, "{arg}")?;
    }

    Ok(())
}
