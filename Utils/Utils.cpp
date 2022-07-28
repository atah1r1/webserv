/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 19:17:39 by ehakam            #+#    #+#             */
/*   Updated: 2022/07/28 20:23:11 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

std::string getFileExtension(const std::string& fileName) {
	int index = fileName.find_last_of(".");
	if (index != -1) {
		std::string ext = fileName.substr(index + 1);
		return ext;
	}
	return "";
}

bool beginsWith(const std::string& haystack, const std::string& needle) {
	if (haystack.empty() || needle.empty()) return false;
	std::string::const_iterator hit = haystack.begin();
	std::string::const_iterator nit = needle.begin();
	while (nit != needle.end())
	{
		if (*nit != *hit) return false;
		++nit; ++hit;
	}
	return true;
}

std::string trim(const std::string& str) {
	if (str.empty()) return str;
	size_t _begin = 0;
	size_t _end = str.length() - 1;
	while (_begin < str.length() && str[_begin] == ' ') ++_begin;
	while (_end > _begin && str[_end] == ' ') --_end;
	return str.substr(_begin, _end - _begin + 1);
}

std::pair<size_t, std::string> nextLine(const std::string& str, size_t start) {
	if (str.empty()) return std::make_pair(0, str);
	size_t _end = start;

	while (_end < str.length() && str[_end] != '\n') ++_end;
	std::string _line = str.substr(start, _end - start);

	if (_end < str.length())
		return std::make_pair(++_end, _line);
	return std::make_pair(_end, _line);
}

Location *_matchLocation( std::vector<Location *> locations, const std::string& path) {
	for (size_t i = 0; i < locations.size(); ++i) {
		if (locations[i] == NULL) continue;
		std::string _loc = trim(locations[i]->_location);
		if (_loc == path) {
			return locations[i];
		}
	}
	return NULL;
}

Location *matchLocation( std::vector<Location *> locations, const std::string& path) {
	std::string _path = trim(path);
	while(!_path.empty()) {
		Location* _l = _matchLocation(locations, _path);
		if (_l != NULL) return _l;
		int _index = _path.find_last_of("/");
		if (_index == -1) continue;
		if (_index == _path.length() - 1) {
			_path = _path.substr(0, _index);
		} else {
			_path = _path.substr(0, _index + 1);
		}
	}
	return NULL;
}

std::string	getLastModificationDate(struct stat st)
{
	tm *ltm = gmtime(&st.st_mtime);

	std::stringstream date;

	date << day_names[ltm->tm_wday] << ", " << ltm->tm_mday << " "
		<< month_names[ltm->tm_mon] << " " << (ltm->tm_year + 1900) << " " 
		<< (ltm->tm_hour) % 24 << ":" << ltm->tm_min << ":" << ltm->tm_sec << " GMT";
	return date.str();
}

std::string	getCurrentDate(void)
{
	time_t now = time(0);
	tm *ltm = gmtime(&now);

	std::stringstream date;

	date << day_names[ltm->tm_wday] << ", " << ltm->tm_mday << " "
		<< month_names[ltm->tm_mon] << " " << (ltm->tm_year + 1900) << " " 
		<< (ltm->tm_hour) % 24 << ":" << ltm->tm_min << ":" << ltm->tm_sec << " GMT";
	return date.str();
}

std::string toHeaderCase(const std::string& header) {
	if (header.empty()) return "";
	std::string _new_header;
	
	for (size_t i = 0; i < header.length(); ++i)
	{
		if (i == 0)
			_new_header.push_back(toupper(header[i]));
		else if (header[i - 1] == '-')
			_new_header.push_back(toupper(header[i]));
		else
			_new_header.push_back(tolower(header[i]));
	}
	return _new_header;
}

FileType getType(const std::string& path) {
	struct stat s;

	if( stat(path.c_str(), &s) == 0 )
	{
		if( s.st_mode & S_IFDIR )
			return T_DIR;
		else if( s.st_mode & S_IFREG )
			return T_FILE;
		else
			return T_OTHER;
	}
	return T_ERROR;
}

void _fillMimes( void ) {
	if (!mimes.empty()) return;
		std::map<std::string, std::string> mimes;

	mimes.insert(std::make_pair("*3gpp", "audio/3gpp"));
	mimes.insert(std::make_pair("*jpm", "video/jpm"));
	mimes.insert(std::make_pair("*mp3", "audio/mp3"));
	mimes.insert(std::make_pair("*rtf", "text/rtf"));
	mimes.insert(std::make_pair("*wav", "audio/wave"));
	mimes.insert(std::make_pair("*xml", "text/xml"));
	mimes.insert(std::make_pair("3g2", "video/3gpp2"));
	mimes.insert(std::make_pair("3gp", "video/3gpp"));
	mimes.insert(std::make_pair("3gpp", "video/3gpp"));
	mimes.insert(std::make_pair("ac", "application/pkix-attr-cert"));
	mimes.insert(std::make_pair("adp", "audio/adpcm"));
	mimes.insert(std::make_pair("ai", "application/postscript"));
	mimes.insert(std::make_pair("apng", "image/apng"));
	mimes.insert(std::make_pair("appcache", "text/cache-manifest"));
	mimes.insert(std::make_pair("asc", "application/pgp-signature"));
	mimes.insert(std::make_pair("atom", "application/atom+xml"));
	mimes.insert(std::make_pair("atomcat", "application/atomcat+xml"));
	mimes.insert(std::make_pair("atomsvc", "application/atomsvc+xml"));
	mimes.insert(std::make_pair("au", "audio/basic"));
	mimes.insert(std::make_pair("aw", "application/applixware"));
	mimes.insert(std::make_pair("bdoc", "application/bdoc"));
	mimes.insert(std::make_pair("bin", "application/octet-stream"));
	mimes.insert(std::make_pair("bmp", "image/bmp"));
	mimes.insert(std::make_pair("bpk", "application/octet-stream"));
	mimes.insert(std::make_pair("buffer", "application/octet-stream"));
	mimes.insert(std::make_pair("ccxml", "application/ccxml+xml"));
	mimes.insert(std::make_pair("cdmia", "application/cdmi-capability"));
	mimes.insert(std::make_pair("cdmic", "application/cdmi-container"));
	mimes.insert(std::make_pair("cdmid", "application/cdmi-domain"));
	mimes.insert(std::make_pair("cdmio", "application/cdmi-object"));
	mimes.insert(std::make_pair("cdmiq", "application/cdmi-queue"));
	mimes.insert(std::make_pair("cer", "application/pkix-cert"));
	mimes.insert(std::make_pair("cgm", "image/cgm"));
	mimes.insert(std::make_pair("class", "application/java-vm"));
	mimes.insert(std::make_pair("coffee", "text/coffeescript"));
	mimes.insert(std::make_pair("conf", "text/plain"));
	mimes.insert(std::make_pair("cpt", "application/mac-compactpro"));
	mimes.insert(std::make_pair("crl", "application/pkix-crl"));
	mimes.insert(std::make_pair("css", "text/css"));
	mimes.insert(std::make_pair("csv", "text/csv"));
	mimes.insert(std::make_pair("cu", "application/cu-seeme"));
	mimes.insert(std::make_pair("davmount", "application/davmount+xml"));
	mimes.insert(std::make_pair("dbk", "application/docbook+xml"));
	mimes.insert(std::make_pair("deb", "application/octet-stream"));
	mimes.insert(std::make_pair("def", "text/plain"));
	mimes.insert(std::make_pair("deploy", "application/octet-stream"));
	mimes.insert(std::make_pair("disposition-notification", "message/disposition-notification"));
	mimes.insert(std::make_pair("dist", "application/octet-stream"));
	mimes.insert(std::make_pair("distz", "application/octet-stream"));
	mimes.insert(std::make_pair("dll", "application/octet-stream"));
	mimes.insert(std::make_pair("dmg", "application/octet-stream"));
	mimes.insert(std::make_pair("dms", "application/octet-stream"));
	mimes.insert(std::make_pair("doc", "application/msword"));
	mimes.insert(std::make_pair("dot", "application/msword"));
	mimes.insert(std::make_pair("drle", "image/dicom-rle"));
	mimes.insert(std::make_pair("dssc", "application/dssc+der"));
	mimes.insert(std::make_pair("dtd", "application/xml-dtd"));
	mimes.insert(std::make_pair("dump", "application/octet-stream"));
	mimes.insert(std::make_pair("ear", "application/java-archive"));
	mimes.insert(std::make_pair("ecma", "application/ecmascript"));
	mimes.insert(std::make_pair("elc", "application/octet-stream"));
	mimes.insert(std::make_pair("emf", "image/emf"));
	mimes.insert(std::make_pair("eml", "message/rfc822"));
	mimes.insert(std::make_pair("emma", "application/emma+xml"));
	mimes.insert(std::make_pair("eps", "application/postscript"));
	mimes.insert(std::make_pair("epub", "application/epub+zip"));
	mimes.insert(std::make_pair("es", "application/ecmascript"));
	mimes.insert(std::make_pair("exe", "application/octet-stream"));
	mimes.insert(std::make_pair("exi", "application/exi"));
	mimes.insert(std::make_pair("exr", "image/aces"));
	mimes.insert(std::make_pair("ez", "application/andrew-inset"));
	mimes.insert(std::make_pair("fits", "image/fits"));
	mimes.insert(std::make_pair("g3", "image/g3fax"));
	mimes.insert(std::make_pair("gbr", "application/rpki-ghostbusters"));
	mimes.insert(std::make_pair("geojson", "application/geo+json"));
	mimes.insert(std::make_pair("gif", "image/gif"));
	mimes.insert(std::make_pair("glb", "model/gltf-binary"));
	mimes.insert(std::make_pair("gltf", "model/gltf+json"));
	mimes.insert(std::make_pair("gml", "application/gml+xml"));
	mimes.insert(std::make_pair("gpx", "application/gpx+xml"));
	mimes.insert(std::make_pair("gram", "application/srgs"));
	mimes.insert(std::make_pair("grxml", "application/srgs+xml"));
	mimes.insert(std::make_pair("gxf", "application/gxf"));
	mimes.insert(std::make_pair("gz", "application/gzip"));
	mimes.insert(std::make_pair("h261", "video/h261"));
	mimes.insert(std::make_pair("h263", "video/h263"));
	mimes.insert(std::make_pair("h264", "video/h264"));
	mimes.insert(std::make_pair("heic", "image/heic"));
	mimes.insert(std::make_pair("heics", "image/heic-sequence"));
	mimes.insert(std::make_pair("heif", "image/heif"));
	mimes.insert(std::make_pair("heifs", "image/heif-sequence"));
	mimes.insert(std::make_pair("hjson", "application/hjson"));
	mimes.insert(std::make_pair("hlp", "application/winhlp"));
	mimes.insert(std::make_pair("hqx", "application/mac-binhex40"));
	mimes.insert(std::make_pair("htm", "text/html"));
	mimes.insert(std::make_pair("html", "text/html"));
	mimes.insert(std::make_pair("ico", "image/x-icon"));
	mimes.insert(std::make_pair("ics", "text/calendar"));
	mimes.insert(std::make_pair("ief", "image/ief"));
	mimes.insert(std::make_pair("ifb", "text/calendar"));
	mimes.insert(std::make_pair("iges", "model/iges"));
	mimes.insert(std::make_pair("igs", "model/iges"));
	mimes.insert(std::make_pair("img", "application/octet-stream"));
	mimes.insert(std::make_pair("in", "text/plain"));
	mimes.insert(std::make_pair("ini", "text/plain"));
	mimes.insert(std::make_pair("ink", "application/inkml+xml"));
	mimes.insert(std::make_pair("inkml", "application/inkml+xml"));
	mimes.insert(std::make_pair("ipfix", "application/ipfix"));
	mimes.insert(std::make_pair("iso", "application/octet-stream"));
	mimes.insert(std::make_pair("jade", "text/jade"));
	mimes.insert(std::make_pair("jar", "application/java-archive"));
	mimes.insert(std::make_pair("jls", "image/jls"));
	mimes.insert(std::make_pair("jp2", "image/jp2"));
	mimes.insert(std::make_pair("jpe", "image/jpeg"));
	mimes.insert(std::make_pair("jpeg", "image/jpeg"));
	mimes.insert(std::make_pair("jpf", "image/jpx"));
	mimes.insert(std::make_pair("jpg", "image/jpeg"));
	mimes.insert(std::make_pair("jpg2", "image/jp2"));
	mimes.insert(std::make_pair("jpgm", "video/jpm"));
	mimes.insert(std::make_pair("jpgv", "video/jpeg"));
	mimes.insert(std::make_pair("jpm", "image/jpm"));
	mimes.insert(std::make_pair("jpx", "image/jpx"));
	mimes.insert(std::make_pair("js", "application/javascript"));
	mimes.insert(std::make_pair("json", "application/json"));
	mimes.insert(std::make_pair("json5", "application/json5"));
	mimes.insert(std::make_pair("jsonld", "application/ld+json"));
	mimes.insert(std::make_pair("jsonml", "application/jsonml+json"));
	mimes.insert(std::make_pair("jsx", "text/jsx"));
	mimes.insert(std::make_pair("kar", "audio/midi"));
	mimes.insert(std::make_pair("ktx", "image/ktx"));
	mimes.insert(std::make_pair("less", "text/less"));
	mimes.insert(std::make_pair("list", "text/plain"));
	mimes.insert(std::make_pair("litcoffee", "text/coffeescript"));
	mimes.insert(std::make_pair("log", "text/plain"));
	mimes.insert(std::make_pair("lostxml", "application/lost+xml"));
	mimes.insert(std::make_pair("lrf", "application/octet-stream"));
	mimes.insert(std::make_pair("m1v", "video/mpeg"));
	mimes.insert(std::make_pair("m21", "application/mp21"));
	mimes.insert(std::make_pair("m2a", "audio/mpeg"));
	mimes.insert(std::make_pair("m2v", "video/mpeg"));
	mimes.insert(std::make_pair("m3a", "audio/mpeg"));
	mimes.insert(std::make_pair("m4a", "audio/mp4"));
	mimes.insert(std::make_pair("m4p", "application/mp4"));
	mimes.insert(std::make_pair("ma", "application/mathematica"));
	mimes.insert(std::make_pair("mads", "application/mads+xml"));
	mimes.insert(std::make_pair("man", "text/troff"));
	mimes.insert(std::make_pair("manifest", "text/cache-manifest"));
	mimes.insert(std::make_pair("map", "application/json"));
	mimes.insert(std::make_pair("mar", "application/octet-stream"));
	mimes.insert(std::make_pair("markdown", "text/markdown"));
	mimes.insert(std::make_pair("mathml", "application/mathml+xml"));
	mimes.insert(std::make_pair("mb", "application/mathematica"));
	mimes.insert(std::make_pair("mbox", "application/mbox"));
	mimes.insert(std::make_pair("md", "text/markdown"));
	mimes.insert(std::make_pair("me", "text/troff"));
	mimes.insert(std::make_pair("mesh", "model/mesh"));
	mimes.insert(std::make_pair("meta4", "application/metalink4+xml"));
	mimes.insert(std::make_pair("metalink", "application/metalink+xml"));
	mimes.insert(std::make_pair("mets", "application/mets+xml"));
	mimes.insert(std::make_pair("mft", "application/rpki-manifest"));
	mimes.insert(std::make_pair("mid", "audio/midi"));
	mimes.insert(std::make_pair("midi", "audio/midi"));
	mimes.insert(std::make_pair("mime", "message/rfc822"));
	mimes.insert(std::make_pair("mj2", "video/mj2"));
	mimes.insert(std::make_pair("mjp2", "video/mj2"));
	mimes.insert(std::make_pair("mjs", "application/javascript"));
	mimes.insert(std::make_pair("mml", "text/mathml"));
	mimes.insert(std::make_pair("mods", "application/mods+xml"));
	mimes.insert(std::make_pair("mov", "video/quicktime"));
	mimes.insert(std::make_pair("mp2", "audio/mpeg"));
	mimes.insert(std::make_pair("mp21", "application/mp21"));
	mimes.insert(std::make_pair("mp2a", "audio/mpeg"));
	mimes.insert(std::make_pair("mp3", "audio/mpeg"));
	mimes.insert(std::make_pair("mp4", "video/mp4"));
	mimes.insert(std::make_pair("mp4a", "audio/mp4"));
	mimes.insert(std::make_pair("mp4s", "application/mp4"));
	mimes.insert(std::make_pair("mp4v", "video/mp4"));
	mimes.insert(std::make_pair("mpd", "application/dash+xml"));
	mimes.insert(std::make_pair("mpe", "video/mpeg"));
	mimes.insert(std::make_pair("mpeg", "video/mpeg"));
	mimes.insert(std::make_pair("mpg", "video/mpeg"));
	mimes.insert(std::make_pair("mpg4", "video/mp4"));
	mimes.insert(std::make_pair("mpga", "audio/mpeg"));
	mimes.insert(std::make_pair("mrc", "application/marc"));
	mimes.insert(std::make_pair("mrcx", "application/marcxml+xml"));
	mimes.insert(std::make_pair("ms", "text/troff"));
	mimes.insert(std::make_pair("mscml", "application/mediaservercontrol+xml"));
	mimes.insert(std::make_pair("msh", "model/mesh"));
	mimes.insert(std::make_pair("msi", "application/octet-stream"));
	mimes.insert(std::make_pair("msm", "application/octet-stream"));
	mimes.insert(std::make_pair("msp", "application/octet-stream"));
	mimes.insert(std::make_pair("mxf", "application/mxf"));
	mimes.insert(std::make_pair("mxml", "application/xv+xml"));
	mimes.insert(std::make_pair("n3", "text/n3"));
	mimes.insert(std::make_pair("nb", "application/mathematica"));
	mimes.insert(std::make_pair("oda", "application/oda"));
	mimes.insert(std::make_pair("oga", "audio/ogg"));
	mimes.insert(std::make_pair("ogg", "audio/ogg"));
	mimes.insert(std::make_pair("ogv", "video/ogg"));
	mimes.insert(std::make_pair("ogx", "application/ogg"));
	mimes.insert(std::make_pair("omdoc", "application/omdoc+xml"));
	mimes.insert(std::make_pair("onepkg", "application/onenote"));
	mimes.insert(std::make_pair("onetmp", "application/onenote"));
	mimes.insert(std::make_pair("onetoc", "application/onenote"));
	mimes.insert(std::make_pair("onetoc2", "application/onenote"));
	mimes.insert(std::make_pair("opf", "application/oebps-package+xml"));
	mimes.insert(std::make_pair("otf", "font/otf"));
	mimes.insert(std::make_pair("owl", "application/rdf+xml"));
	mimes.insert(std::make_pair("oxps", "application/oxps"));
	mimes.insert(std::make_pair("p10", "application/pkcs10"));
	mimes.insert(std::make_pair("p7c", "application/pkcs7-mime"));
	mimes.insert(std::make_pair("p7m", "application/pkcs7-mime"));
	mimes.insert(std::make_pair("p7s", "application/pkcs7-signature"));
	mimes.insert(std::make_pair("p8", "application/pkcs8"));
	mimes.insert(std::make_pair("pdf", "application/pdf"));
	mimes.insert(std::make_pair("pfr", "application/font-tdpfr"));
	mimes.insert(std::make_pair("pgp", "application/pgp-encrypted"));
	mimes.insert(std::make_pair("pkg", "application/octet-stream"));
	mimes.insert(std::make_pair("pki", "application/pkixcmp"));
	mimes.insert(std::make_pair("pkipath", "application/pkix-pkipath"));
	mimes.insert(std::make_pair("pls", "application/pls+xml"));
	mimes.insert(std::make_pair("png", "image/png"));
	mimes.insert(std::make_pair("prf", "application/pics-rules"));
	mimes.insert(std::make_pair("ps", "application/postscript"));
	mimes.insert(std::make_pair("pskcxml", "application/pskc+xml"));
	mimes.insert(std::make_pair("qt", "video/quicktime"));
	mimes.insert(std::make_pair("raml", "application/raml+yaml"));
	mimes.insert(std::make_pair("rdf", "application/rdf+xml"));
	mimes.insert(std::make_pair("rif", "application/reginfo+xml"));
	mimes.insert(std::make_pair("rl", "application/resource-lists+xml"));
	mimes.insert(std::make_pair("rld", "application/resource-lists-diff+xml"));
	mimes.insert(std::make_pair("rmi", "audio/midi"));
	mimes.insert(std::make_pair("rnc", "application/relax-ng-compact-syntax"));
	mimes.insert(std::make_pair("rng", "application/xml"));
	mimes.insert(std::make_pair("roa", "application/rpki-roa"));
	mimes.insert(std::make_pair("roff", "text/troff"));
	mimes.insert(std::make_pair("rq", "application/sparql-query"));
	mimes.insert(std::make_pair("rs", "application/rls-services+xml"));
	mimes.insert(std::make_pair("rsd", "application/rsd+xml"));
	mimes.insert(std::make_pair("rss", "application/rss+xml"));
	mimes.insert(std::make_pair("rtf", "application/rtf"));
	mimes.insert(std::make_pair("rtx", "text/richtext"));
	mimes.insert(std::make_pair("s3m", "audio/s3m"));
	mimes.insert(std::make_pair("sbml", "application/sbml+xml"));
	mimes.insert(std::make_pair("scq", "application/scvp-cv-request"));
	mimes.insert(std::make_pair("scs", "application/scvp-cv-response"));
	mimes.insert(std::make_pair("sdp", "application/sdp"));
	mimes.insert(std::make_pair("ser", "application/java-serialized-object"));
	mimes.insert(std::make_pair("setpay", "application/set-payment-initiation"));
	mimes.insert(std::make_pair("setreg", "application/set-registration-initiation"));
	mimes.insert(std::make_pair("sgi", "image/sgi"));
	mimes.insert(std::make_pair("sgm", "text/sgml"));
	mimes.insert(std::make_pair("sgml", "text/sgml"));
	mimes.insert(std::make_pair("shex", "text/shex"));
	mimes.insert(std::make_pair("shf", "application/shf+xml"));
	mimes.insert(std::make_pair("shtml", "text/html"));
	mimes.insert(std::make_pair("sig", "application/pgp-signature"));
	mimes.insert(std::make_pair("sil", "audio/silk"));
	mimes.insert(std::make_pair("silo", "model/mesh"));
	mimes.insert(std::make_pair("slim", "text/slim"));
	mimes.insert(std::make_pair("slm", "text/slim"));
	mimes.insert(std::make_pair("smi", "application/smil+xml"));
	mimes.insert(std::make_pair("smil", "application/smil+xml"));
	mimes.insert(std::make_pair("snd", "audio/basic"));
	mimes.insert(std::make_pair("so", "application/octet-stream"));
	mimes.insert(std::make_pair("spp", "application/scvp-vp-response"));
	mimes.insert(std::make_pair("spq", "application/scvp-vp-request"));
	mimes.insert(std::make_pair("spx", "audio/ogg"));
	mimes.insert(std::make_pair("sru", "application/sru+xml"));
	mimes.insert(std::make_pair("srx", "application/sparql-results+xml"));
	mimes.insert(std::make_pair("ssdl", "application/ssdl+xml"));
	mimes.insert(std::make_pair("ssml", "application/ssml+xml"));
	mimes.insert(std::make_pair("stk", "application/hyperstudio"));
	mimes.insert(std::make_pair("styl", "text/stylus"));
	mimes.insert(std::make_pair("stylus", "text/stylus"));
	mimes.insert(std::make_pair("svg", "image/svg+xml"));
	mimes.insert(std::make_pair("svgz", "image/svg+xml"));
	mimes.insert(std::make_pair("t", "text/troff"));
	mimes.insert(std::make_pair("t38", "image/t38"));
	mimes.insert(std::make_pair("tei", "application/tei+xml"));
	mimes.insert(std::make_pair("teicorpus", "application/tei+xml"));
	mimes.insert(std::make_pair("text", "text/plain"));
	mimes.insert(std::make_pair("tfi", "application/thraud+xml"));
	mimes.insert(std::make_pair("tfx", "image/tiff-fx"));
	mimes.insert(std::make_pair("tif", "image/tiff"));
	mimes.insert(std::make_pair("tiff", "image/tiff"));
	mimes.insert(std::make_pair("tr", "text/troff"));
	mimes.insert(std::make_pair("ts", "video/mp2t"));
	mimes.insert(std::make_pair("tsd", "application/timestamped-data"));
	mimes.insert(std::make_pair("tsv", "text/tab-separated-values"));
	mimes.insert(std::make_pair("ttc", "font/collection"));
	mimes.insert(std::make_pair("ttf", "font/ttf"));
	mimes.insert(std::make_pair("ttl", "text/turtle"));
	mimes.insert(std::make_pair("txt", "text/plain"));
	mimes.insert(std::make_pair("u8dsn", "message/global-delivery-status"));
	mimes.insert(std::make_pair("u8hdr", "message/global-headers"));
	mimes.insert(std::make_pair("u8mdn", "message/global-disposition-notification"));
	mimes.insert(std::make_pair("u8msg", "message/global"));
	mimes.insert(std::make_pair("uri", "text/uri-list"));
	mimes.insert(std::make_pair("uris", "text/uri-list"));
	mimes.insert(std::make_pair("urls", "text/uri-list"));
	mimes.insert(std::make_pair("vcard", "text/vcard"));
	mimes.insert(std::make_pair("vrml", "model/vrml"));
	mimes.insert(std::make_pair("vtt", "text/vtt"));
	mimes.insert(std::make_pair("vxml", "application/voicexml+xml"));
	mimes.insert(std::make_pair("war", "application/java-archive"));
	mimes.insert(std::make_pair("wasm", "application/wasm"));
	mimes.insert(std::make_pair("wav", "audio/wav"));
	mimes.insert(std::make_pair("weba", "audio/webm"));
	mimes.insert(std::make_pair("webm", "video/webm"));
	mimes.insert(std::make_pair("webmanifest", "application/manifest+json"));
	mimes.insert(std::make_pair("webp", "image/webp"));
	mimes.insert(std::make_pair("wgt", "application/widget"));
	mimes.insert(std::make_pair("wmf", "image/wmf"));
	mimes.insert(std::make_pair("woff", "font/woff"));
	mimes.insert(std::make_pair("woff2", "font/woff2"));
	mimes.insert(std::make_pair("wrl", "model/vrml"));
	mimes.insert(std::make_pair("wsdl", "application/wsdl+xml"));
	mimes.insert(std::make_pair("wspolicy", "application/wspolicy+xml"));
	mimes.insert(std::make_pair("x3d", "model/x3d+xml"));
	mimes.insert(std::make_pair("x3db", "model/x3d+binary"));
	mimes.insert(std::make_pair("x3dbz", "model/x3d+binary"));
	mimes.insert(std::make_pair("x3dv", "model/x3d+vrml"));
	mimes.insert(std::make_pair("x3dvz", "model/x3d+vrml"));
	mimes.insert(std::make_pair("x3dz", "model/x3d+xml"));
	mimes.insert(std::make_pair("xaml", "application/xaml+xml"));
	mimes.insert(std::make_pair("xdf", "application/xcap-diff+xml"));
	mimes.insert(std::make_pair("xdssc", "application/dssc+xml"));
	mimes.insert(std::make_pair("xenc", "application/xenc+xml"));
	mimes.insert(std::make_pair("xer", "application/patch-ops-error+xml"));
	mimes.insert(std::make_pair("xht", "application/xhtml+xml"));
	mimes.insert(std::make_pair("xhtml", "application/xhtml+xml"));
	mimes.insert(std::make_pair("xhvml", "application/xv+xml"));
	mimes.insert(std::make_pair("xm", "audio/xm"));
	mimes.insert(std::make_pair("xml", "application/xml"));
	mimes.insert(std::make_pair("xop", "application/xop+xml"));
	mimes.insert(std::make_pair("xpl", "application/xproc+xml"));
	mimes.insert(std::make_pair("xsd", "application/xml"));
	mimes.insert(std::make_pair("xsl", "application/xml"));
	mimes.insert(std::make_pair("xslt", "application/xslt+xml"));
	mimes.insert(std::make_pair("xspf", "application/xspf+xml"));
	mimes.insert(std::make_pair("xvm", "application/xv+xml"));
	mimes.insert(std::make_pair("xvml", "application/xv+xml"));
	mimes.insert(std::make_pair("yaml", "text/yaml"));
	mimes.insert(std::make_pair("yang", "application/yang"));
	mimes.insert(std::make_pair("yin", "application/yin+xml"));
	mimes.insert(std::make_pair("yml", "text/yaml"));
	mimes.insert(std::make_pair("zip", "application/zip"));
}

std::string getMimeType(const std::string& extension) {
	_fillMimes();
	std::map<std::string, std::string>::iterator it = mimes.find(extension);
	if (it != mimes.end()) {
		return it->second;
	}
	return "";	
}

std::string toUpperCase(const std::string& str) {
	std::string _s;
	
	for (size_t i = 0; i < str.length(); ++i)
		_s.push_back(toupper(str[i]));
	return _s;
}

std::string toLowerCase(const std::string& str) {
	std::string _s;
	
	for (size_t i = 0; i < str.length(); ++i)
		_s.push_back(tolower(str[i]));
	return _s;
}

bool isMethodAllowed(std::vector<std::string> allowedMethods, const std::string& method) {
	std::string _m = toUpperCase(trim(method));

	std::vector<std::string>::iterator it = allowedMethods.begin();
	while (it != allowedMethods.end()) {
		if (_m == toUpperCase(trim(*it))) {
			return true;
		}
	}
	return false;
}

bool isMethodImplemented(const std::string& method) {
	std::string _m = toUpperCase(trim(method));

	for (size_t i = 0; i < 3; ++i) {
		if (_m == http_methods[i])
			return true;
	}
	return false;
}

inline bool pathExists( const std::string& path ) {
	return ( access( path.c_str(), F_OK ) != -1 );
}

std::string getFullPath( const std::string& root, const std::string& path ) {
	std::string _root = trim(root);
	std::string _path = trim(path);

	if (_root.back() == '/' && _path.front() == '/') {
		return _root.substr(0, _root.length() - 1).append(_path);
	} else if (_root.back() != '/' && _path.front() != '/') {
		return _root.append("/").append(_path);
	}
	return _root.append(_path);
}

