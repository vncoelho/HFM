// OptFrame - Optimization Framework

// Copyright (C) 2009-2015
// http://optframe.sourceforge.net/
//
// This file is part of the OptFrame optimization framework. This framework
// is free software; you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License v3 as published by the
// Free Software Foundation.

// This framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License v3 for more details.

// You should have received a copy of the GNU Lesser General Public License v3
// along with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
// USA.

#ifndef OPTFRAME_SYSTEM_ASSERT_MODULE_HPP_
#define OPTFRAME_SYSTEM_ASSERT_MODULE_HPP_

#include "../Command.hpp"

namespace optframe
{

template<class R, class ADS = OPTFRAME_DEFAULT_ADS, class DS = OPTFRAME_DEFAULT_DS>
class SystemAssertCommand: public Command<R, ADS, DS>
{
public:

	virtual ~SystemAssertCommand()
	{

	}

	string id()
	{
		return "system.assert";
	}

	string usage()
	{
		return "system.assert boolean";
	}

	bool run(vector<Command<R, ADS, DS>*>&, vector<PreprocessFunction<R, ADS, DS>*>&, HeuristicFactory<R, ADS, DS>&, map<string, string>&, map<string, vector<string> >&, string input)
	{
		Scanner scanner(input);

		if (!scanner.hasNext())
		{
			cout << "module " << id() << " usage: " << usage() << endl;
			return false;
		}

		string boolean = scanner.next();

		if (boolean == "true")
			return true;
		else if (boolean == "false")
			return false;
		else
		{
			cout << "module " << id() << " error: no such boolean value '" << boolean << "'" << endl;
			return false;
		}
	}

	virtual string* preprocess(vector<PreprocessFunction<R, ADS, DS>*>& allFunctions, HeuristicFactory<R, ADS, DS>& hf, const map<string, string>& dictionary, const map<string, vector<string> >& ldictionary, string input)
	{
		return Command<R, ADS, DS>::defaultPreprocess(allFunctions, hf, dictionary, ldictionary, input);
	}

};

}

#endif /* OPTFRAME_SYSTEM_ASSERT_MODULE_HPP_ */
