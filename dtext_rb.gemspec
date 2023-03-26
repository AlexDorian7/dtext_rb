# frozen_string_literal: true

Gem::Specification.new do |s|
  s.name = "dtext_rb"
  s.version = "1.10.21"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.require_paths = ["lib"]
  s.authors = ["r888888888"]
  s.date = "2021-07-08"
  s.description = "Compiled DText parser"
  s.email = "r888888888@gmail.com"
  s.executables = ["dtext"]
  s.extensions = ["ext/dtext/extconf.rb"]
  s.extra_rdoc_files = [
    "README.md"
  ]
  s.files = [
    "bin/dtext",
    "lib/dtext.rb",
    "lib/dtext/dtext.so",
    "lib/dtext_ruby.rb"
  ]
  s.homepage = "http://github.com/r888888888/dtext_rb"
  s.licenses = ["MIT"]
  s.rubygems_version = "3.3.20"
  s.summary = "Compiled DText parser"

  if s.respond_to? :specification_version then
    s.specification_version = 4
  end

  if s.respond_to? :add_runtime_dependency then
    s.add_runtime_dependency(%q<nokogiri>, ["~> 1.8"])
    s.add_development_dependency(%q<minitest>, ["~> 5.10"])
    s.add_development_dependency(%q<rake-compiler>, ["~> 1.0"])
  else
    s.add_dependency(%q<nokogiri>, ["~> 1.8"])
    s.add_dependency(%q<minitest>, ["~> 5.10"])
    s.add_dependency(%q<rake-compiler>, ["~> 1.0"])
  end
end
