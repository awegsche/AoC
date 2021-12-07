use std::str::FromStr;

use proc_macro::TokenStream;
use quote::{quote, quote_spanned};
use std::fmt::Display;
use syn::{parse_macro_input, Attribute, DeriveInput, Ident, Lit, Macro, Meta, MetaNameValue};

use aoc::day::Day;

/// -------------------------------------------------------------------------------------------------
/// ---- proc macro ---------------------------------------------------------------------------------
/// -------------------------------------------------------------------------------------------------

#[proc_macro_derive(Day, attributes(day, year, title, part1, part2))]
pub fn derive_day(input: TokenStream) -> TokenStream {
    let DeriveInput {
        ident, data, attrs, ..
    } = parse_macro_input!(input);

    match derive_day_inner(&ident, &attrs) {
        Ok(tokenstream) => tokenstream,
        Err(err) => {
            let output = quote_spanned! {
            ident.span() => compile_error!{format!("#[year=YEAR] error: {}", err)}};

            output.into()
        }
    }
}

fn derive_day_inner(ident: &Ident, attrs: &[Attribute]) -> anyhow::Result<TokenStream> {
    let day = get_meta_int::<u8>(&attrs, "day")?;
    let year = get_meta_str(&attrs, "year")?;

    let title = if let Ok(title) = get_meta_str(&attrs, "title") {
        quote! {
            fn title() -> &'static str {
                #title
            }

        }
    } else {
        quote! {
            fn title() -> &'static str {
                ""
            }
        }
    };

    let tests = get_tests(ident, &attrs)?;

    let output = quote! {
    impl Day for #ident {
        fn day() -> u8 {
            #day
        }

        fn year() -> &'static str {
            #year
        }

        #title
    }

        #tests
    };

    Ok(output.into())
}

fn get_tests(ident: &Ident, attrs: &[Attribute]) -> anyhow::Result<quote::__private::TokenStream> {
    let part1 = get_meta_unparsed(attrs, "part1")?;
    let part2 = get_meta_unparsed(attrs, "part2")?;

    let output = quote! {

        #[cfg(test)]
        mod tests {

            use super::*;

            #[test]
            fn part1() {
                println!("{:?}", std::env::current_dir().unwrap().to_str());
                println!(stringify!(assert_eq!(#ident::from_test().unwrap().part1().unwrap(), #part1);));
                assert_eq!(
                    #ident::from_test().unwrap().part1().unwrap(), #part1
                );
            }

            #[test]
            fn part2() {
                assert_eq!(
                    #ident::from_test().unwrap().part2().unwrap(), #part2
                );
            }
        }
    };

    Ok(output)
}

fn find_att<'a>(attrs: &'a [Attribute], name: &str) -> Option<&'a Attribute> {
    attrs.iter().find(|att| att.path.is_ident(name))
}

fn get_meta_str(attrs: &[Attribute], name: &str) -> Result<String, anyhow::Error> {
    let attr = find_att(attrs, name);

    if attr.is_none() {
        return MacroError::err(format!("missing attribute #[{}=VALUE]", name));
    }
    let attr = attr.unwrap();

    match attr.parse_meta()? {
        Meta::NameValue(metaname) => match metaname.lit {
            Lit::Str(lit) => Ok(lit.value()),
            _ => MacroError::err(format!(
                "attribute #[{}=VALUE] needs to be a literal string",
                name
            )),
        },
        _ => MacroError::err(format!("attribute syntax: #[{}=VALUE]", name)),
    }
}

fn get_meta_unparsed(attrs: &[Attribute], name: &str) -> Result<Lit, anyhow::Error> {
    let attr = find_att(attrs, name);

    if attr.is_none() {
        return MacroError::err("no attribute defined");
    }
    let attr = attr.unwrap();

    if let Meta::NameValue(name) = attr.parse_meta()? {
        Ok(name.lit)
    } else {
        MacroError::err("expected #[name=value]")
    }
}

fn get_meta_int<T>(attrs: &[Attribute], name: &str) -> Result<T, anyhow::Error>
where
    T: FromStr,
    <T as FromStr>::Err: std::fmt::Display,
{
    let attr = find_att(attrs, name);

    if attr.is_none() {
        return MacroError::err("no attribute defined");
    }
    let attr = attr.unwrap();

    match attr.parse_meta()? {
        Meta::NameValue(name) => match name.lit {
            Lit::Int(lit) => Ok(lit.base10_parse()?),
            _ => MacroError::err("attribute needs to be a literal integer"),
        },
        _ => MacroError::err("attribute needs to be a literal integer"),
    }
}

#[derive(Debug)]
struct MacroError {
    message: String,
}

impl MacroError {
    pub fn new<S: Into<String>>(mess: S) -> Self {
        Self {
            message: mess.into(),
        }
    }

    pub fn err<T, S: Into<String>>(mess: S) -> Result<T, anyhow::Error> {
        Err(MacroError::new(mess).into())
    }
}

impl std::error::Error for MacroError {}

impl Display for MacroError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.message)
    }
}
