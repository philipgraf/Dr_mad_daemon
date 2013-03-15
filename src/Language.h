
#ifndef LANGUAGE_H_
#define LANGUAGE_H_

#include <yaml-cpp/yaml.h>
#include <string>
#include <vector>

/** Language class.
 * This class handels all translations.
 * @author Philip Graf
 * @date 14.03.2013
 * @version 0.1.0 Alpha-State
 */
class Language {
public:
	static std::vector<std::string> supLanguages; /**< a list with all supported Languages */
private:
	YAML::Node root; /**< the root node of the lang.yml file */

public:
	Language();
	std::string operator[](std::string key);
	static void getSupportedLanguages();

};

#endif /* LANGUAGE_H_ */
