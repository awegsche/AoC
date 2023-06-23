use atomic_refcell::AtomicRefCell;
use lazy_static::lazy_static;

use log::{Level, Metadata, Record, SetLoggerError};

lazy_static! {
    static ref LOGGER: AocLogger = AocLogger::new();
}

struct AocLogger {
    logs: AtomicRefCell<Vec<String>>
}

impl AocLogger {
    pub fn new() -> Self {
        Self {
            logs: AtomicRefCell::new(Vec::new())
        }
    }

    pub fn extract_logs(&self) -> Vec<String> {
        self.logs.borrow_mut().drain(..).collect()
    }
}

impl log::Log for AocLogger {
    fn enabled(&self, metadata: &log::Metadata) -> bool {
        true
    }

    fn log(&self, record: &log::Record) {
        self.logs.borrow_mut().push(format!("{}", record.args()));
    }

    fn flush(&self) {

    }
}

pub fn init() -> Result<(), SetLoggerError>{
    log::set_logger(&*LOGGER).map(|()| log::set_max_level(log::LevelFilter::Debug))
}

pub fn extract_logs() -> Vec<String> {
    LOGGER.extract_logs()
}
