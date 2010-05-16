/*--------------------------------------------------------------------------------*
 * Name: Parser Look-up tree
 * Desc: function and data structures to speed up a specific string search
 *
 *         **** DO NOT AMEND THIS CODE - IT IS AUTO_GENERATED ****
 *
 * Code and table produced by:
 *            build_graph 
 *            version 0.3
 *
 *  written by Peter Antoine. 
 *
 *   **** This code can be used however you like, no claims whatsoever are ****
 *   **** placed on the auto-generated code or data, or on the code that   ****
 *   **** uses the that code or data.                                      ****
 *   **** Or, to paraphrase "Fill you're boots".                         ****
 *--------------------------------------------------------------------------------*/

#ifndef	__ICAL_STRINGS.H_H__
#define __ICAL_STRINGS.H_H__

typedef	struct
{
	char*			name;
	unsigned int	length;
} ICAL_STRINGS.H_STRING_TABLE;

typedef enum
{
	TST_VCALENDAR,
	TST_VEVENT,
	TST_VTODO,
	TST_VJOURNAL,
	TST_VFREEBUSY,
	TST_VTIMEZONE,
	TST_VALARM,
	TST_STANDARD,
	TST_DAYLIGHT,
	TST_CALSCALE,
	TST_METHOD,
	TST_PRODID,
	TST_VERSION,
	TST_ATTACH,
	TST_CATEGORIES,
	TST_CLASS,
	TST_COMMENT,
	TST_DESCRIPTION,
	TST_GEO,
	TST_LOCATION,
	TST_PERCENT_COMPLETE,
	TST_PRIORITY,
	TST_RESOURCES,
	TST_STATUS,
	TST_SUMMARY,
	TST_COMPLETED,
	TST_DTEND,
	TST_DUE,
	TST_DTSTART,
	TST_DURATION,
	TST_FREEBUSY,
	TST_TRANSP,
	TST_TZID,
	TST_TZNAME,
	TST_TZOFFSETFROM,
	TST_TZOFFSETTO,
	TST_TZURL,
	TST_ATTENDEE,
	TST_CONTACT,
	TST_ORGANIZER,
	TST_RECURRENCE_ID,
	TST_RELATED_TO,
	TST_URL,
	TST_UID,
	TST_EXDATE,
	TST_EXRULE,
	TST_RDATE,
	TST_RRULE,
	TST_ACTION,
	TST_REPEAT,
	TST_TRIGGER,
	TST_CREATED,
	TST_DTSTAMP,
	TST_LAST_MODIFIED,
	TST_SEQUENCE,
	TST_REQUEST_STATUS,
	TST_ALTREP,
	TST_CN,
	TST_CUTYPE,
	TST_DELEGATED_FROM,
	TST_DELEGATED_TO,
	TST_DIR,
	TST_ENCODING,
	TST_FMTTYPE,
	TST_FBTYPE,
	TST_LANGUAGE,
	TST_MEMBER,
	TST_PARTSTAT,
	TST_RANGE,
	TST_RELATED,
	TST_RELTYPE,
	TST_ROLE,
	TST_RSVP,
	TST_SENT_BY,
	TST_TZID,
	TST_BINARY,
	TST_BOOLEAN,
	TST_CAL_ADDRESS,
	TST_DATE,
	TST_DATE_TIME,
	TST_DURATION,
	TST_FLOAT,
	TST_INTEGER,
	TST_PERIOD,
	TST_RECUR,
	TST_TEXT,
	TST_TIME,
	TST_URI,
	TST_UTC_OFFSET,
	TST_INDIVIDUAL,
	TST_GROUP,
	TST_RESOURCE,
	TST_ROOM,
	TST_UNKNOWN,
	TST_FREE,
	TST_BUSY,
	TST_BUSY_UNAVAILABLE,
	TST_BUSY_TENTATIVE,
	TST_NEEDS_ACTION,
	TST_ACCEPTED,
	TST_DECLINED,
	TST_TENTATIVE,
	TST_DELEGATED,
	TST_COMPLETED,
	TST_IN_PROCESS,
	TST_CHILD,
	TST_PARENT,
	TST_SIBLING,
	TST_CHAIR,
	TST_REQ_PARTICIPANT,
	TST_OPT_PARTICIPANT,
	TST_NON_PARTICIPANT,
	TST_AUDIO,
	TST_DISPLAY,
	TST_EMAIL,
	TST_PROCEDURE,
	TST_PUBLIC,
	TST_PRIVATE,
	TST_CONFIDENTIAL,
	TST_8BIT,
	TST_BASE64	
} TST_ICAL_STRINGS.H;

int	ical_strings.h_check_word(char* word);

#endif
