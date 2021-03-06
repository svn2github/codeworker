/* "CodeWorker":	a scripting language for parsing and generating text.

Copyright (C) 1996-1997, 1999-2010 C�dric Lemaire

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

To contact the author: codeworker@free.fr
*/

#ifdef WIN32
#pragma warning (disable : 4786)
#endif

#include "ScpStream.h"
#include "CppCompilerEnvironment.h"
#include "CGRuntime.h"
#include "ExprScriptExpression.h"
#include <string>

//##protect##"INCLUDE FILES"
#include "DtaScriptVariable.h"
//##protect##"INCLUDE FILES"

#include "GrfWriteTextOnce.h"

namespace CodeWorker {
	GrfWriteTextOnce::~GrfWriteTextOnce() {
		delete _pText;
	}

	SEQUENCE_INTERRUPTION_LIST GrfWriteTextOnce::executeInternal(DtaScriptVariable& visibility) {
		std::string sText = _pText->getValue(visibility);
//##protect##"execute"
		if (*_pOutputCoverage != NULL) {
			int iLocation = CGRuntime::getOutputLocation();
			if (CGRuntime::getOutputStream()->writeTextOnce(sText)) {
				DtaScriptVariable* pCoverage = (*_pOutputCoverage)->pushItem("W");
				pCoverage->insertNode("script")->setValue(_iFileLocation);
				pCoverage->insertNode("output")->setValue(iLocation + (*_pOutputCoverage)->getIntValue());
			}
			return NO_INTERRUPTION;
		}
//##protect##"execute"
		return CGRuntime::writeTextOnce(sText);
	}

//##protect##"implementation"
	void GrfWriteTextOnce::prepareCoverage(DtaScriptVariable* const* pOutputCoverage) {
		_pOutputCoverage = pOutputCoverage;
	}
//##protect##"implementation"

	void GrfWriteTextOnce::compileCpp(CppCompilerEnvironment& theCompilerEnvironment) const {
		CW_BODY_INDENT << "CGRuntime::writeTextOnce(";
		_pText->compileCppString(theCompilerEnvironment);
		CW_BODY_STREAM << ");";
		CW_BODY_ENDL;
	}
}
