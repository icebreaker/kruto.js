require 'sinatra'

set :public, File.join(File.dirname(__FILE__), '..')

get '/' do
  File.read('index.html')
end

run Sinatra::Application
