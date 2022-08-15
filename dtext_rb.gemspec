Gem::Specification.new do |s|
  s.name = "dtext_rb".freeze
  s.version = "1.10.19"

  s.required_rubygems_version = Gem::Requirement.new(">= 0".freeze) if s.respond_to? :required_rubygems_version=
  s.require_paths = ["lib".freeze]
  s.authors = ["r888888888".freeze]
  s.date = "2021-07-08"
  s.description = "Compiled DText parser".freeze
  s.email = "r888888888@gmail.com".freeze
  s.executables = ["cdtext".freeze, "dtext".freeze]
  s.extensions = ["ext/dtext/extconf.rb".freeze]
  s.extra_rdoc_files = [
    "README.md"
  ]
  s.files = [
    "bin/cdtext",
    "bin/dtext",
    "lib/dtext.rb",
    "lib/dtext/dtext.so",
    "lib/dtext_ruby.rb"
  ]
  s.homepage = "http://github.com/r888888888/dtext_rb".freeze
  s.licenses = ["MIT".freeze]
  s.rubygems_version = "3.1.6".freeze
  s.summary = "Compiled DText parser".freeze

  if s.respond_to? :specification_version then
    s.specification_version = 4
  end

  if s.respond_to? :add_runtime_dependency then
    s.add_runtime_dependency(%q<nokogiri>.freeze, ["~> 1.8"])
    s.add_development_dependency(%q<minitest>.freeze, ["~> 5.10"])
    s.add_development_dependency(%q<rake-compiler>.freeze, ["~> 1.0"])
  else
    s.add_dependency(%q<nokogiri>.freeze, ["~> 1.8"])
    s.add_dependency(%q<minitest>.freeze, ["~> 5.10"])
    s.add_dependency(%q<rake-compiler>.freeze, ["~> 1.0"])
  end
end
