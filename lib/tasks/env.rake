# frozen_string_literal: true

require 'pathname'
task :env do
  @path = 'client'
  @app_root = Pathname("#{__dir__}/../..")
end