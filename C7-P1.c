#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TITLE_LEN 64
#define MAX_DATE_LEN 11   // "YYYY-MM-DD" + null
#define MAX_FIELD_LEN 32
#define MAX_QUAL_LEN 128
#define MAX_SNS 5
#define MAX_API_LEN 64
#define MAX_NAME_LEN 32

// --- Data Structures ---

typedef struct {
    int id;
    char title[MAX_TITLE_LEN];
    char posting_date[MAX_DATE_LEN];
    char deadline[MAX_DATE_LEN];
} JobAdListEntry;

typedef struct {
    int id;
    char title[MAX_TITLE_LEN];
    char posting_date[MAX_DATE_LEN];
    char deadline[MAX_DATE_LEN];
    int number_of_hires;
    char job_field[MAX_FIELD_LEN];
    int qual_count;
    char **qualifications; // Array of strings
} JobAdDetail;

typedef struct JobAdNode {
    JobAdDetail *detail;
    struct JobAdNode *next;
} JobAdNode;

typedef struct {
    char sns_name[MAX_NAME_LEN];
    char api_key[MAX_API_LEN];
    char api_url[MAX_API_LEN];
} SocialNetworkAPI;

// --- Global Variables ---

static int next_job_id = 1;
static JobAdNode *job_list_head = NULL;
static JobAdNode *expired_list_head = NULL;

// SNS API info
static SocialNetworkAPI sns_list[MAX_SNS] = {
    {"Facebook", "FB-API-KEY", "https://api.facebook.com/post"},
    {"Instagram", "IG-API-KEY", "https://api.instagram.com/post"},
    {"Thread", "THREAD-API-KEY", "https://api.thread.com/post"},
    {"LinkedIn", "LI-API-KEY", "https://api.linkedin.com/post"},
    {"X", "X-API-KEY", "https://api.x.com/post"}
};

// --- Utility Functions ---

void strip_newline(char *s) {
    char *p = strchr(s, '\n');
    if (p) *p = 0;
}
void pause() {
    printf("Press Enter to continue...");
    getchar();
}

// Parse date string "YYYY-MM-DD" to struct tm, returns 0 on success
int parse_date(const char *datestr, struct tm *out) {
    int y, m, d;
    if (sscanf(datestr, "%4d-%2d-%2d", &y, &m, &d) != 3) return -1;
    memset(out, 0, sizeof(struct tm));
    out->tm_year = y - 1900;
    out->tm_mon = m - 1;
    out->tm_mday = d;
    return 0;
}

// Returns 1 if date1 < date2, 0 if not (expired if deadline < today)
int is_expired(const char *deadline, const char *today) {
    struct tm d1, d2;
    if (parse_date(deadline, &d1) != 0 || parse_date(today, &d2) != 0) return 0;
    time_t t1 = mktime(&d1);
    time_t t2 = mktime(&d2);
    return (difftime(t1, t2) < 0.0 ? 1 : 0);
}

// Get today's date in "YYYY-MM-DD"
void get_today(char *out) {
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    strftime(out, MAX_DATE_LEN, "%Y-%m-%d", tm_now);
}

// --- Dynamic Memory Management ---

// Free one JobAdDetail
void free_job_detail(JobAdDetail *d) {
    if (!d) return;
    for (int i = 0; i < d->qual_count; ++i)
        free(d->qualifications[i]);
    free(d->qualifications);
    free(d);
}
// Free a whole JobAdNode list
void free_job_list(JobAdNode *head) {
    JobAdNode *cur = head;
    while (cur) {
        JobAdNode *tmp = cur;
        cur = cur->next;
        free_job_detail(tmp->detail);
        free(tmp);
    }
}

// --- Job Posting Menu Logic ---

void input_with_limit(const char *prompt, char *dst, int maxlen) {
    printf("%s", prompt);
    if (fgets(dst, maxlen, stdin) == NULL) dst[0] = 0;
    strip_newline(dst);
}

// Create a new job posting (allocates and adds to job_list_head)
void create_job_posting() {
    char buf[MAX_QUAL_LEN];
    JobAdDetail *d = (JobAdDetail*)malloc(sizeof(JobAdDetail));
    if (!d) { printf("Memory error\n"); return; }
    d->id = next_job_id++;
    input_with_limit("Job Title: ", d->title, MAX_TITLE_LEN);
    input_with_limit("Posting Date (YYYY-MM-DD): ", d->posting_date, MAX_DATE_LEN);
    input_with_limit("Deadline (YYYY-MM-DD): ", d->deadline, MAX_DATE_LEN);
    printf("Number of Hires: ");
    fgets(buf, sizeof(buf), stdin); d->number_of_hires = atoi(buf);
    input_with_limit("Job Field: ", d->job_field, MAX_FIELD_LEN);

    // Qualifications (variable count)
    printf("Enter number of qualifications: ");
    fgets(buf, sizeof(buf), stdin);
    d->qual_count = atoi(buf);
    if (d->qual_count < 1) d->qual_count = 1;

    d->qualifications = (char**)malloc(sizeof(char*) * d->qual_count);
    for (int i = 0; i < d->qual_count; ++i) {
        printf("Qualification #%d: ", i + 1);
        fgets(buf, sizeof(buf), stdin); strip_newline(buf);
        d->qualifications[i] = (char*)malloc(strlen(buf) + 1);
        strcpy(d->qualifications[i], buf);
    }

    // Add to job_list_head
    JobAdNode *n = (JobAdNode*)malloc(sizeof(JobAdNode));
    n->detail = d;
    n->next = job_list_head;
    job_list_head = n;

    printf("Job posting created successfully! [ID: %d]\n", d->id);
    pause();
}

// List job postings (active or expired)
void list_job_postings(JobAdNode *head, int expired_only) {
    char today[MAX_DATE_LEN];
    get_today(today);
    int found = 0;
    JobAdNode *cur = head;
    printf("\n%-5s | %-25s | %-12s | %-12s\n", "ID", "Title", "Posting Date", "Deadline");
    printf("---------------------------------------------------------------\n");
    while (cur) {
        int expired = is_expired(cur->detail->deadline, today);
        if ((expired_only && expired) || (!expired_only && !expired)) {
            printf("%-5d | %-25s | %-12s | %-12s\n",
                   cur->detail->id, cur->detail->title, cur->detail->posting_date, cur->detail->deadline);
            found = 1;
        }
        cur = cur->next;
    }
    if (!found)
        printf("No %s job postings.\n", expired_only ? "expired" : "active");
}

// Find job by ID in given list
JobAdDetail* find_job_by_id(JobAdNode *head, int id, int expired_only) {
    char today[MAX_DATE_LEN];
    get_today(today);
    JobAdNode *cur = head;
    while (cur) {
        int expired = is_expired(cur->detail->deadline, today);
        if (cur->detail->id == id && ((expired_only && expired) || (!expired_only && !expired))) {
            return cur->detail;
        }
        cur = cur->next;
    }
    return NULL;
}

void view_job_postings() {
    char buf[16];
    char today[MAX_DATE_LEN];
    get_today(today);

    // Check if any job postings exist
    if (!job_list_head) {
        printf("No job postings found. Redirecting to Create Job Posting...\n");
        pause();
        create_job_posting();
        return;
    }
    while (1) {
        printf("\n[1] View Active Postings\n[2] View Expired Postings\n[3] Back\nSelect: ");
        fgets(buf, sizeof(buf), stdin);
        int sel = atoi(buf);
        if (sel == 1 || sel == 2) {
            int expired_only = (sel == 2);
            list_job_postings(job_list_head, expired_only);
            printf("\nEnter Job ID to view details or 0 to go back: ");
            fgets(buf, sizeof(buf), stdin);
            int id = atoi(buf);
            if (id == 0) continue;
            JobAdDetail *d = find_job_by_id(job_list_head, id, expired_only);
            if (!d) {
                printf("Invalid Job ID.\n");
                pause();
            } else {
                printf("\n--- Job Posting Detail ---\n");
                printf("ID: %d\nTitle: %s\nPosting Date: %s\nDeadline: %s\nNumber of Hires: %d\nJob Field: %s\nQualifications:\n",
                    d->id, d->title, d->posting_date, d->deadline, d->number_of_hires, d->job_field);
                for (int i = 0; i < d->qual_count; ++i)
                    printf("  - %s\n", d->qualifications[i]);
                pause();
            }
        } else if (sel == 3) {
            return;
        } else {
            printf("Invalid selection.\n");
        }
    }
}

// --- Social Network Posting (Stub) ---

void post_on_social_network() {
    char buf[16];
    if (!job_list_head) {
        printf("No job postings found. Redirecting to Create Job Posting...\n");
        pause();
        create_job_posting();
    }
    // List active job postings
    list_job_postings(job_list_head, 0);
    printf("\nEnter Job ID to post (0 to cancel): ");
    fgets(buf, sizeof(buf), stdin);
    int id = atoi(buf);
    if (id == 0) return;
    JobAdDetail *d = find_job_by_id(job_list_head, id, 0);
    if (!d) {
        printf("Invalid Job ID.\n");
        pause();
        return;
    }
    printf("\nSelect Social Network:\n");
    for (int i = 0; i < MAX_SNS; ++i)
        printf("  %d. %s\n", i+1, sns_list[i].sns_name);
    printf("Select: ");
    fgets(buf, sizeof(buf), stdin);
    int sn = atoi(buf) - 1;
    if (sn < 0 || sn >= MAX_SNS) {
        printf("Invalid selection.\n");
        pause();
        return;
    }
    // Simulate posting (stub)
    printf("\nPosting to %s...\n", sns_list[sn].sns_name);
    printf("Job ID: %d\nAPI Key: %s\nAPI URL: %s\n",
           d->id, sns_list[sn].api_key, sns_list[sn].api_url);
    printf("Job Title: %s\n", d->title);
    printf("... Success! Job posted to %s.\n", sns_list[sn].sns_name);
    pause();
}

// --- Main Menu for findSpecialist() ---

void findSpecialist() {
    char buf[16];
    while (1) {
        printf("\n=== People Finder ===\n");
        printf("1. Create Job Posting\n");
        printf("2. View Job Postings\n");
        printf("3. Post on Social Networks\n");
        printf("4. Exit\n");
        printf("Select: ");
        fgets(buf, sizeof(buf), stdin);
        switch (buf[0]) {
        case '1': create_job_posting(); break;
        case '2': view_job_postings(); break;
        case '3': post_on_social_network(); break;
        case '4': free_job_list(job_list_head); return;
        default: printf("Invalid selection.\n");
        }
    }
}
