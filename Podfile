# Uncomment this line to define a global platform for your project
# platform :ios, '6.0'

target :SlimEAS do
  pod 'expat', '~> 2.1'
  pod 'libwbxml', '~> 0.11.2'
  pod 'mimetic', '~> 0.9.7'
end

target :SlimEASDemo do
  pod 'pop', '~> 1.0.7'
end

post_install do |installer|
  
  default_library = installer.libraries.detect { |i| i.target_definition.name == :SlimEAS }
  debugConfig_path = default_library.library.xcconfig_path('Debug')
  releaseConfig_path = default_library.library.xcconfig_path('Release')
  
  File.open("debug_config.tmp", "w") { |io|
    config = File.read(debugConfig_path)
    config = config.gsub(/-l\"stdc\+\+\"/, '')
    io << config
  }
 	FileUtils.mv("debug_config.tmp", debugConfig_path)
 	
  File.open("release_config.tmp", "w") { |io|
 		config = File.read(releaseConfig_path)
  	config = config.gsub(/-l\"stdc\+\+\"/, '')
  	io << config
	}
  FileUtils.mv("release_config.tmp", releaseConfig_path)
end

