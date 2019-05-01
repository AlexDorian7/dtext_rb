require "dtext/dtext"
require "nokogiri"

module DTextRagel
  class Error < StandardError; end

  def self.parse_inline(str)
    parse(str, :inline => true)
  end

  def self.parse_strip(str)
    parse(str, :strip => true)
  end

  def self.parse(str, strip: false, inline: false, disable_mentions: false, base_url: nil, max_thumbs: 25)
    html = c_parse(str, strip, inline, disable_mentions, max_thumbs)
    html[0] = resolve_relative_urls(html[0], base_url) if base_url
    html
  end

  private

  def self.resolve_relative_urls(html, base_url)
    nodes = Nokogiri::HTML.fragment(html)
    nodes.traverse do |node|
      if node[:href]&.start_with?("/")
        node[:href] = base_url.chomp("/") + node[:href]
      end
    end
    nodes.to_s
  end
end
