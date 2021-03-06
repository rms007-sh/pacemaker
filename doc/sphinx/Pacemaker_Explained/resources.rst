.. _resource:

Cluster Resources
-----------------

.. Convert_to_RST:
   
   [[s-resource-primitive]]
   == What is a Cluster Resource? ==
   
   indexterm:[Resource]
   
   A resource is a service made highly available by a cluster.
   The simplest type of resource, a 'primitive' resource, is described
   in this chapter. More complex forms, such as groups and clones,
   are described in later chapters.
   
   Every primitive resource has a 'resource agent'. A resource agent is an
   external program that abstracts the service it provides and present a
   consistent view to the cluster.
   
   This allows the cluster to be agnostic about the resources it manages.
   The cluster doesn't need to understand how the resource works because
   it relies on the resource agent to do the right thing when given a
   `start`, `stop` or `monitor` command. For this reason, it is crucial that
   resource agents are well-tested.
   
   Typically, resource agents come in the form of shell scripts. However,
   they can be written using any technology (such as C, Python or Perl)
   that the author is comfortable with.
   
   [[s-resource-supported]]
   == Resource Classes ==
   
   indexterm:[Resource,class]
   
   Pacemaker supports several classes of agents:
   
   * OCF
   * LSB
   * Upstart
   * Systemd
   * Service
   * Fencing
   * Nagios Plugins
   
   === Open Cluster Framework ===
   
   indexterm:[Resource,OCF]
   indexterm:[OCF,Resources]
   indexterm:[Open Cluster Framework,Resources]
   
   The OCF standard
   footnote:[See https://github.com/ClusterLabs/OCF-spec/tree/master/ra . The
   Pacemaker implementation has been somewhat extended from the OCF specs.]
   is basically an extension of the Linux Standard Base conventions for
   init scripts to:
   
   * support parameters,
   * make them self-describing, and
   * make them extensible
   
   OCF specs have strict definitions of the exit codes that actions must return.
   footnote:[
   The resource-agents source code includes the `ocf-tester` script, which
   can be useful in this regard.
   ]
   
   The cluster follows these specifications exactly, and giving the wrong
   exit code will cause the cluster to behave in ways you will likely
   find puzzling and annoying.  In particular, the cluster needs to
   distinguish a completely stopped resource from one which is in some
   erroneous and indeterminate state.
   
   Parameters are passed to the resource agent as environment variables, with the
   special prefix +OCF_RESKEY_+.  So, a parameter which the user thinks
   of as +ip+ will be passed to the resource agent as +OCF_RESKEY_ip+.  The
   number and purpose of the parameters is left to the resource agent; however,
   the resource agent should use the `meta-data` command to advertise any that it
   supports.
   
   The OCF class is the most preferred as it is an industry standard,
   highly flexible (allowing parameters to be passed to agents in a
   non-positional manner) and self-describing.
   
   For more information, see the
   http://www.linux-ha.org/wiki/OCF_Resource_Agents[reference] and
   the 'Resource Agents' chapter of 'Pacemaker Administration'.
   
   === Linux Standard Base ===
   indexterm:[Resource,LSB]
   indexterm:[LSB,Resources]
   indexterm:[Linux Standard Base,Resources]
   
   'LSB' resource agents are more commonly known as 'init scripts'. If a full path
   is not given, they are assumed to be located in +/etc/init.d+.
   
   Commonly, they are provided by the OS distribution. In order to be used
   with a Pacemaker cluster, they must conform to the LSB specification.
   footnote:[
   See
   http://refspecs.linux-foundation.org/LSB_3.0.0/LSB-Core-generic/LSB-Core-generic/iniscrptact.html
   for the LSB Spec as it relates to init scripts.
   ]
   
   [WARNING]
   ====
   Many distributions or particular software packages claim LSB compliance
   but ship with broken init scripts.  For details on how to check whether
   your init script is LSB-compatible, see the 'Resource Agents' chapter of
   'Pacemaker Administration'. Common problematic violations of the LSB
   standard include:
   
   * Not implementing the +status+ operation at all
   * Not observing the correct exit status codes for
     +start+/+stop+/+status+ actions
   * Starting a started resource returns an error
   * Stopping a stopped resource returns an error
   ====
   
   [IMPORTANT]
   ====
   Remember to make sure the computer is _not_ configured to start any
   services at boot time -- that should be controlled by the cluster.
   ====
   
   [[s-resource-supported-systemd]]
   === Systemd ===
   indexterm:[Resource,Systemd]
   indexterm:[Systemd,Resources]
   
   Some newer distributions have replaced the old
   http://en.wikipedia.org/wiki/Init#SysV-style["SysV"] style of
   initialization daemons and scripts with an alternative called
   http://www.freedesktop.org/wiki/Software/systemd[Systemd].
   
   Pacemaker is able to manage these services _if they are present_.
   
   Instead of init scripts, systemd has 'unit files'.  Generally, the
   services (unit files) are provided by the OS distribution, but there
   are online guides for converting from init scripts.
   footnote:[For example,
   http://0pointer.de/blog/projects/systemd-for-admins-3.html]
   
   [IMPORTANT]
   ====
   Remember to make sure the computer is _not_ configured to start any
   services at boot time -- that should be controlled by the cluster.
   ====
   
   === Upstart ===
   indexterm:[Resource,Upstart]
   indexterm:[Upstart,Resources]
   
   Some newer distributions have replaced the old
   http://en.wikipedia.org/wiki/Init#SysV-style["SysV"] style of
   initialization daemons (and scripts) with an alternative called
   http://upstart.ubuntu.com/[Upstart].
   
   Pacemaker is able to manage these services _if they are present_.
   
   Instead of init scripts, upstart has 'jobs'.  Generally, the
   services (jobs) are provided by the OS distribution.
   
   [IMPORTANT]
   ====
   Remember to make sure the computer is _not_ configured to start any
   services at boot time -- that should be controlled by the cluster.
   ====
   
   === System Services ===
   indexterm:[Resource,System Services]
   indexterm:[System Service,Resources]
   
   Since there are various types of system services (+systemd+,
   +upstart+, and +lsb+), Pacemaker supports a special +service+ alias which
   intelligently figures out which one applies to a given cluster node.
   
   This is particularly useful when the cluster contains a mix of
   +systemd+, +upstart+, and +lsb+.
   
   In order, Pacemaker will try to find the named service as:
   
   . an LSB init script
   . a Systemd unit file
   . an Upstart job
   
   === STONITH ===
   indexterm:[Resource,STONITH]
   indexterm:[STONITH,Resources]
   
   The STONITH class is used exclusively for fencing-related resources.  This is
   discussed later in <<ch-fencing>>.
   
   === Nagios Plugins ===
   indexterm:[Resource,Nagios Plugins]
   indexterm:[Nagios Plugins,Resources]
   
   Nagios Plugins
   footnote:[The project has two independent forks, hosted at
   https://www.nagios-plugins.org/ and https://www.monitoring-plugins.org/. Output
   from both projects' plugins is similar, so plugins from either project can be
   used with pacemaker.]
   allow us to monitor services on remote hosts.
   
   Pacemaker is able to do remote monitoring with the plugins _if they are
   present_.
   
   A common use case is to configure them as resources belonging to a resource
   container (usually a virtual machine), and the container will be restarted
   if any of them has failed. Another use is to configure them as ordinary
   resources to be used for monitoring hosts or services via the network.
   
   The supported parameters are same as the long options of the plugin.
   
   [[primitive-resource]]
   == Resource Properties ==
   
   These values tell the cluster which resource agent to use for the resource,
   where to find that resource agent and what standards it conforms to.
   
   .Properties of a Primitive Resource
   [width="95%",cols="1m,<6",options="header",align="center"]
   |=========================================================
   
   |Field
   |Description
   
   |id
   |Your name for the resource
    indexterm:[id,Resource]
    indexterm:[Resource,Property,id]
   
   |class
   
   |The standard the resource agent conforms to. Allowed values:
   +lsb+, +nagios+, +ocf+, +service+, +stonith+, +systemd+, +upstart+
    indexterm:[class,Resource]
    indexterm:[Resource,Property,class]
   
   |type
   |The name of the Resource Agent you wish to use. E.g. +IPaddr+ or +Filesystem+
    indexterm:[type,Resource]
    indexterm:[Resource,Property,type]
   
   |provider
   |The OCF spec allows multiple vendors to supply the same
    resource agent. To use the OCF resource agents supplied by
    the Heartbeat project, you would specify +heartbeat+ here.
    indexterm:[provider,Resource]
    indexterm:[Resource,Property,provider]
   
   |=========================================================
   
   The XML definition of a resource can be queried with the `crm_resource` tool.
   For example:
   
   ----
   # crm_resource --resource Email --query-xml
   ----
   
   might produce:
   
   .A system resource definition
   =====
   [source,XML]
   <primitive id="Email" class="service" type="exim"/>
   =====
   
   [NOTE]
   =====
   One of the main drawbacks to system services (LSB, systemd or
   Upstart) resources is that they do not allow any parameters!
   =====
   
   ////
   See https://tools.ietf.org/html/rfc5737 for choice of example IP address
   ////
   
   .An OCF resource definition
   =====
   [source,XML]
   -------
   <primitive id="Public-IP" class="ocf" type="IPaddr" provider="heartbeat">
      <instance_attributes id="Public-IP-params">
         <nvpair id="Public-IP-ip" name="ip" value="192.0.2.2"/>
      </instance_attributes>
   </primitive>
   -------
   =====

.. _resource_options:

Resource Options
################
   
.. Convert_to_RST_2:

   Resources have two types of options: 'meta-attributes' and 'instance attributes'.
   Meta-attributes apply to any type of resource, while instance attributes
   are specific to each resource agent.
   
   === Resource Meta-Attributes ===
   
   Meta-attributes are used by the cluster to decide how a resource should
   behave and can be easily set using the `--meta` option of the
   `crm_resource` command.
   
   .Meta-attributes of a Primitive Resource
   [width="95%",cols="2m,2,<5",options="header",align="center"]
   |=========================================================
   
   |Field
   |Default
   |Description
   
   |priority
   |0
   |If not all resources can be active, the cluster will stop lower
   priority resources in order to keep higher priority ones active.
   indexterm:[priority,Resource Option]
   indexterm:[Resource,Option,priority]
   
   |target-role
   |Started
   a|What state should the cluster attempt to keep this resource in? Allowed values:
   
   * +Stopped:+ Force the resource to be stopped
   * +Started:+ Allow the resource to be started (and in the case of
     <<s-resource-promotable,promotable clone resources>>, promoted to master if
     appropriate)
   * +Slave:+ Allow the resource to be started, but only in Slave mode if
     the resource is <<s-resource-promotable,promotable>>
   * +Master:+ Equivalent to +Started+
   indexterm:[target-role,Resource Option]
   indexterm:[Resource,Option,target-role]
   
   |is-managed
   |TRUE
   |Is the cluster allowed to start and stop the resource?  Allowed
    values: +true+, +false+
    indexterm:[is-managed,Resource Option]
    indexterm:[Resource,Option,is-managed]
   
   |maintenance
   |FALSE
   |Similar to the +maintenance-mode+ <<s-cluster-options,cluster option>>, but for
    a single resource. If true, the resource will not be started, stopped, or
    monitored on any node. This differs from +is-managed+ in that monitors will
    not be run. Allowed values: +true+, +false+
    indexterm:[maintenance,Resource Option]
    indexterm:[Resource,Option,maintenance]

.. _resource-stickiness:

   placeholder

.. Convert_to_RST_3:

   |resource-stickiness
   |1 for individual clone instances, 0 for all other resources
   |A score that will be added to the current node when a resource is already
    active. This allows running resources to stay where they are, even if
    they would be placed elsewhere if they were being started from a stopped
    state.
    indexterm:[resource-stickiness,Resource Option]
    indexterm:[Resource,Option,resource-stickiness]


.. _requires:

   placeholder

.. Convert_to_RST_4:

   |requires
   |+quorum+ for resources with a +class+ of +stonith+,
    otherwise +unfencing+ if unfencing is active in the cluster,
    otherwise +fencing+ if +stonith-enabled+ is true, otherwise +quorum+
   a|Conditions under which the resource can be started
   Allowed values:
   
   * +nothing:+ can always be started
   * +quorum:+ The cluster can only start this resource if a majority of
     the configured nodes are active
   * +fencing:+ The cluster can only start this resource if a majority
     of the configured nodes are active _and_ any failed or unknown nodes
     have been <<ch-fencing,fenced>>
   * +unfencing:+
     The cluster can only start this resource if a majority
     of the configured nodes are active _and_ any failed or unknown nodes
     have been fenced _and_ only on nodes that have been
     <<s-unfencing,unfenced>>
   
   indexterm:[requires,Resource Option]
   indexterm:[Resource,Option,requires]
   
   |migration-threshold
   |INFINITY
   |How many failures may occur for this resource on a node, before this
    node is marked ineligible to host this resource. A value of 0 indicates that
    this feature is disabled (the node will never be marked ineligible); by
    constrast, the cluster treats INFINITY (the default) as a very large but
    finite number. This option has an effect only if the failed operation
    specifies +on-fail+ as +restart+ (the default), and additionally for
    failed +start+ operations, if the cluster property +start-failure-is-fatal+
    is +false+.
    indexterm:[migration-threshold,Resource Option]
    indexterm:[Resource,Option,migration-threshold]
   
   |failure-timeout
   |0
   |How many seconds to wait before acting as if the failure had not
    occurred, and potentially allowing the resource back to the node on
    which it failed. A value of 0 indicates that this feature is disabled.
    indexterm:[failure-timeout,Resource Option]
    indexterm:[Resource,Option,failure-timeout]
   
   |multiple-active
   |stop_start
   a|What should the cluster do if it ever finds the resource active on
    more than one node? Allowed values:
   
   * +block:+ mark the resource as unmanaged
   * +stop_only:+ stop all active instances and leave them that way
   * +stop_start:+ stop all active instances and start the resource in
     one location only
   
   indexterm:[multiple-active,Resource Option]
   indexterm:[Resource,Option,multiple-active]
   
   |allow-migrate
   |TRUE for ocf:pacemaker:remote resources, FALSE otherwise
   |Whether the cluster should try to "live migrate" this resource when it needs
   to be moved (see <<s-migrating-resources>>)
   
   |container-attribute-target
   |
   |Specific to bundle resources; see <<s-bundle-attributes>>
   
   |remote-node
   |
   |The name of the Pacemaker Remote guest node this resource is associated with,
    if any. If specified, this both enables the resource as a guest node and
    defines the unique name used to identify the guest node. The guest must be
    configured to run the Pacemaker Remote daemon when it is started. +WARNING:+
    This value cannot overlap with any resource or node IDs.
   
   |remote-port
   |3121
   |If +remote-node+ is specified, the port on the guest used for its
    Pacemaker Remote connection. The Pacemaker Remote daemon on the guest must be
    configured to listen on this port.
   
   |remote-addr
   |value of +remote-node+
   |If +remote-node+ is specified, the IP address or hostname used to connect to
    the guest via Pacemaker Remote. The Pacemaker Remote daemon on the guest
    must be configured to accept connections on this address.
   
   |remote-connect-timeout
   |60s
   |If +remote-node+ is specified, how long before a pending guest connection will
    time out.
   
   |=========================================================
   
   As an example of setting resource options, if you performed the following
   commands on an LSB Email resource:
   
   -------
   # crm_resource --meta --resource Email --set-parameter priority --parameter-value 100
   # crm_resource -m -r Email -p multiple-active -v block
   -------
   
   the resulting resource definition might be:
   
   .An LSB resource with cluster options
   =====
   [source,XML]
   -------
   <primitive id="Email" class="lsb" type="exim">
     <meta_attributes id="Email-meta_attributes">
       <nvpair id="Email-meta_attributes-priority" name="priority" value="100"/>
       <nvpair id="Email-meta_attributes-multiple-active" name="multiple-active" value="block"/>
     </meta_attributes>
   </primitive>
   -------
   =====
   
   In addition to the cluster-defined meta-attributes described above, you may
   also configure arbitrary meta-attributes of your own choosing. Most commonly,
   this would be done for use in <<ch-rules,rules>>. For example, an IT department
   might define a custom meta-attribute to indicate which company department each
   resource is intended for. To reduce the chance of name collisions with
   cluster-defined meta-attributes added in the future, it is recommended to use
   a unique, organization-specific prefix for such attributes.
   
   [[s-resource-defaults]]
   === Setting Global Defaults for Resource Meta-Attributes ===
   
   To set a default value for a resource option, add it to the
   +rsc_defaults+ section with `crm_attribute`. For example,
   
   ----
   # crm_attribute --type rsc_defaults --name is-managed --update false
   ----
   
   would prevent the cluster from starting or stopping any of the
   resources in the configuration (unless of course the individual
   resources were specifically enabled by having their +is-managed+ set to
   +true+).
   
   === Resource Instance Attributes ===
   
   The resource agents of some resource classes (lsb, systemd and upstart 'not' among them)
   can be given parameters which determine how they behave and which instance
   of a service they control.
   
   If your resource agent supports parameters, you can add them with the
   `crm_resource` command. For example,
   
   ----
   # crm_resource --resource Public-IP --set-parameter ip --parameter-value 192.0.2.2
   ----
   
   would create an entry in the resource like this:
   
   .An example OCF resource with instance attributes
   =====
   [source,XML]
   -------
   <primitive id="Public-IP" class="ocf" type="IPaddr" provider="heartbeat">
      <instance_attributes id="params-public-ip">
         <nvpair id="public-ip-addr" name="ip" value="192.0.2.2"/>
      </instance_attributes>
   </primitive>
   -------
   =====
   
   For an OCF resource, the result would be an environment variable
   called +OCF_RESKEY_ip+ with a value of +192.0.2.2+.
   
   The list of instance attributes supported by an OCF resource agent can be
   found by calling the resource agent with the `meta-data` command.
   The output contains an XML description of all the supported
   attributes, their purpose and default values.
   
   .Displaying the metadata for the Dummy resource agent template
   =====
   ----
   # export OCF_ROOT=/usr/lib/ocf
   # $OCF_ROOT/resource.d/pacemaker/Dummy meta-data
   ----
   [source,XML]
   -------
   <?xml version="1.0"?>
   <!DOCTYPE resource-agent SYSTEM "ra-api-1.dtd">
   <resource-agent name="Dummy" version="1.0">
   <version>1.0</version>
   
   <longdesc lang="en">
   This is a Dummy Resource Agent. It does absolutely nothing except 
   keep track of whether its running or not.
   Its purpose in life is for testing and to serve as a template for RA writers.
   
   NB: Please pay attention to the timeouts specified in the actions
   section below. They should be meaningful for the kind of resource
   the agent manages. They should be the minimum advised timeouts,
   but they shouldn't/cannot cover _all_ possible resource
   instances. So, try to be neither overly generous nor too stingy,
   but moderate. The minimum timeouts should never be below 10 seconds.
   </longdesc>
   <shortdesc lang="en">Example stateless resource agent</shortdesc>
   
   <parameters>
   <parameter name="state" unique="1">
   <longdesc lang="en">
   Location to store the resource state in.
   </longdesc>
   <shortdesc lang="en">State file</shortdesc>
   <content type="string" default="/var/run/Dummy-default.state" />
   </parameter>
   
   <parameter name="fake" unique="0">
   <longdesc lang="en">
   Fake attribute that can be changed to cause a reload
   </longdesc>
   <shortdesc lang="en">Fake attribute that can be changed to cause a reload</shortdesc>
   <content type="string" default="dummy" />
   </parameter>
   
   <parameter name="op_sleep" unique="1">
   <longdesc lang="en">
   Number of seconds to sleep during operations.  This can be used to test how
   the cluster reacts to operation timeouts.
   </longdesc>
   <shortdesc lang="en">Operation sleep duration in seconds.</shortdesc>
   <content type="string" default="0" />
   </parameter>
   
   </parameters>
   
   <actions>
   <action name="start"        timeout="20" />
   <action name="stop"         timeout="20" />
   <action name="monitor"      timeout="20" interval="10" depth="0"/>
   <action name="reload"       timeout="20" />
   <action name="migrate_to"   timeout="20" />
   <action name="migrate_from" timeout="20" />
   <action name="validate-all" timeout="20" />
   <action name="meta-data"    timeout="5" />
   </actions>
   </resource-agent>
   -------
   =====


.. _operation:

Resource Operations
###################


.. Convert_to_RST_5:
   
   indexterm:[Resource,Action]
   
   'Operations' are actions the cluster can perform on a resource by calling the
   resource agent. Resource agents must support certain common operations such as
   start, stop, and monitor, and may implement any others.
   
   Operations may be explicitly configured for two purposes: to override defaults
   for options (such as timeout) that the cluster will use whenever it initiates
   the operation, and to run an operation on a recurring basis (for example, to
   monitor the resource for failure).
   
   .An OCF resource with a non-default start timeout
   =====
   [source,XML]
   -------
   <primitive id="Public-IP" class="ocf" type="IPaddr" provider="heartbeat">
     <operations>
        <op id="Public-IP-start" name="start" timeout="60s"/>
     </operations>
     <instance_attributes id="params-public-ip">
        <nvpair id="public-ip-addr" name="ip" value="192.0.2.2"/>
     </instance_attributes>
   </primitive>
   -------
   =====
   
   Pacemaker identifies operations by a combination of name and interval, so this
   combination must be unique for each resource. That is, you should not configure
   two operations for the same resource with the same name and interval.

.. _operation_properties:

Operation Properties
____________________

.. Convert_to_RST_6:

   Operation properties may be specified directly in the +op+ element as
   XML attributes, or in a separate +meta_attributes+ block as +nvpair+ elements.
   XML attributes take precedence over +nvpair+ elements if both are specified.
   
   .Properties of an Operation
   [width="95%",cols="2m,3,<6",options="header",align="center"]
   |=========================================================
   
   |Field
   |Default
   |Description
   
   |id
   |
   |A unique name for the operation.
    indexterm:[id,Action Property]
    indexterm:[Action,Property,id]
   
   |name
   |
   |The action to perform. This can be any action supported by the agent; common
    values include +monitor+, +start+, and +stop+.
    indexterm:[name,Action Property]
    indexterm:[Action,Property,name]
   
   |interval
   |0
   |How frequently (in seconds) to perform the operation. A value of 0 means "when
    needed". A positive value defines a 'recurring action', which is typically
    used with <<s-resource-monitoring,monitor>>.
    indexterm:[interval,Action Property]
    indexterm:[Action,Property,interval]
   
   |timeout
   |
   |How long to wait before declaring the action has failed
    indexterm:[timeout,Action Property]
    indexterm:[Action,Property,timeout]
   
   |on-fail
   a|Varies by action:
   
   * +stop+: +fence+ if +stonith-enabled+ is true or +block+ otherwise
   * +demote+: +on-fail+ of the +monitor+ action with +role+ set to +Master+, if
     present, enabled, and configured to a value other than +demote+, or +restart+
     otherwise
   * all other actions: +restart+
   a|The action to take if this action ever fails. Allowed values:
   
   * +ignore:+ Pretend the resource did not fail.
   * +block:+ Don't perform any further operations on the resource.
   * +stop:+ Stop the resource and do not start it elsewhere.
   * +demote:+ Demote the resource, without a full restart. This is valid only for
     +promote+ actions, and for +monitor+ actions with both a nonzero +interval+
     and +role+ set to +Master+; for any other action, a configuration error will
     be logged, and the default behavior will be used.
   * +restart:+ Stop the resource and start it again (possibly on a different node).
   * +fence:+ STONITH the node on which the resource failed.
   * +standby:+ Move _all_ resources away from the node on which the resource failed.
   
   indexterm:[on-fail,Action Property]
   indexterm:[Action,Property,on-fail]
   
   |enabled
   |TRUE
   |If +false+, ignore this operation definition.  This is typically used to pause
    a particular recurring +monitor+ operation; for instance, it can complement
    the respective resource being unmanaged (+is-managed=false+), as this alone
    will <<s-monitoring-unmanaged,not block any configured monitoring>>.
    Disabling the operation does not suppress all actions of the given type.
    Allowed values: +true+, +false+.
    indexterm:[enabled,Action Property]
    indexterm:[Action,Property,enabled]
   
   |record-pending
   |TRUE
   |If +true+, the intention to perform the operation is recorded so that
    GUIs and CLI tools can indicate that an operation is in progress.
    This is best set as an _operation default_ (see <<s-operation-defaults>>).
    Allowed values: +true+, +false+.
    indexterm:[enabled,Action Property]
    indexterm:[Action,Property,enabled]
   
   |role
   |
   |Run the operation only on node(s) that the cluster thinks should be in
    the specified role. This only makes sense for recurring +monitor+ operations.
    Allowed (case-sensitive) values: +Stopped+, +Started+, and in the
    case of <<s-resource-promotable,promotable clone resources>>, +Slave+ and +Master+.
    indexterm:[role,Action Property]
    indexterm:[Action,Property,role]
   
   |=========================================================
   
   [NOTE]
   ====
   When +on-fail+ is set to +demote+, recovery from failure by a successful demote
   causes the cluster to recalculate whether and where a new instance should be
   promoted. The node with the failure is eligible, so if master scores have not
   changed, it will be promoted again.
   
   There is no direct equivalent of +migration-threshold+ for the master role, but
   the same effect can be achieved with a location constraint using a
   <<ch-rules,rule>> with a node attribute expression for the resource's fail
   count.
   
   For example, to immediately ban the master role from a node with any failed
   promote or master monitor:
   [source,XML]
   ----
   <rsc_location id="loc1" rsc="my_primitive">
       <rule id="rule1" score="-INFINITY" role="Master" boolean-op="or">
         <expression id="expr1" attribute="fail-count-my_primitive#promote_0"
           operation="gte" value="1"/>
         <expression id="expr2" attribute="fail-count-my_primitive#monitor_10000"
           operation="gte" value="1"/>
       </rule>
   </rsc_location>
   ----
   
   This example assumes that there is a promotable clone of the +my_primitive+
   resource (note that the primitive name, not the clone name, is used in the
   rule), and that there is a recurring 10-second-interval monitor configured for
   the master role (fail count attributes specify the interval in milliseconds).
   ====
   
   [[s-resource-monitoring]]
   === Monitoring Resources for Failure ===
   
   When Pacemaker first starts a resource, it runs one-time +monitor+ operations
   (referred to as 'probes') to ensure the resource is running where it's
   supposed to be, and not running where it's not supposed to be. (This behavior
   can be affected by the +resource-discovery+ location constraint property.)
   
   Other than those initial probes, Pacemaker will 'not' (by default) check that
   the resource continues to stay healthy.
   footnote:[Currently, anyway. Automatic monitoring operations may be
   added in a future version of Pacemaker.]
   You must configure +monitor+ operations explicitly to perform these checks.
   
   .An OCF resource with a recurring health check
   =====
   [source,XML]
   -------
   <primitive id="Public-IP" class="ocf" type="IPaddr" provider="heartbeat">
     <operations>
        <op id="Public-IP-start" name="start" timeout="60s"/>
        <op id="Public-IP-monitor" name="monitor" interval="60s"/>
     </operations>
     <instance_attributes id="params-public-ip">
        <nvpair id="public-ip-addr" name="ip" value="192.0.2.2"/>
     </instance_attributes>
   </primitive>
   -------
   =====
   
   By default, a +monitor+ operation will ensure that the resource is running
   where it is supposed to. The +target-role+ property can be used for further
   checking.
   
   For example, if a resource has one +monitor+ operation with
   +interval=10 role=Started+ and a second +monitor+ operation with
   +interval=11 role=Stopped+, the cluster will run the first monitor on any nodes
   it thinks 'should' be running the resource, and the second monitor on any nodes
   that it thinks 'should not' be running the resource (for the truly paranoid,
   who want to know when an administrator manually starts a service by mistake).
   
   [NOTE]
   ====
   Currently, monitors with +role=Stopped+ are not implemented for
   <<s-resource-clone,clone>> resources.
   ====
   
   [[s-monitoring-unmanaged]]
   === Monitoring Resources When Administration is Disabled ===
   
   Recurring +monitor+ operations behave differently under various administrative
   settings:
   
   * When a resource is unmanaged (by setting +is-managed=false+): No monitors
     will be stopped.
   +
   If the unmanaged resource is stopped on a node where the cluster thinks it
   should be running, the cluster will detect and report that it is not, but it
   will not consider the monitor failed, and will not try to start the resource
   until it is managed again.
   +
   Starting the unmanaged resource on a different node is strongly discouraged
   and will at least cause the cluster to consider the resource failed, and
   may require the resource's +target-role+ to be set to +Stopped+ then +Started+
   to be recovered.
   
   * When a node is put into standby: All resources will be moved away from the
     node, and all +monitor+ operations will be stopped on the node, except those
     specifying +role+ as +Stopped+ (which will be newly initiated if
     appropriate).
   
   * When the cluster is put into maintenance mode: All resources will be marked
     as unmanaged. All monitor operations will be stopped, except those
     specifying +role+ as +Stopped+ (which will be newly initiated if
     appropriate). As with single unmanaged resources, starting
     a resource on a node other than where the cluster expects it to be will
     cause problems.
   
   [[s-operation-defaults]]
   === Setting Global Defaults for Operations ===
   
   You can change the global default values for operation properties
   in a given cluster. These are defined in an +op_defaults+ section 
   of the CIB's +configuration+ section, and can be set with `crm_attribute`.
   For example,
   
   ----
   # crm_attribute --type op_defaults --name timeout --update 20s
   ----
   
   would default each operation's +timeout+ to 20 seconds.  If an
   operation's definition also includes a value for +timeout+, then that
   value would be used for that operation instead.
   
   === When Implicit Operations Take a Long Time ===
   
   The cluster will always perform a number of implicit operations: +start+,
   +stop+ and a non-recurring +monitor+ operation used at startup to check
   whether the resource is already active.  If one of these is taking too long,
   then you can create an entry for them and specify a longer timeout.
   
   .An OCF resource with custom timeouts for its implicit actions
   =====
   [source,XML]
   -------
   <primitive id="Public-IP" class="ocf" type="IPaddr" provider="heartbeat">
     <operations>
        <op id="public-ip-startup" name="monitor" interval="0" timeout="90s"/>
        <op id="public-ip-start" name="start" interval="0" timeout="180s"/>
        <op id="public-ip-stop" name="stop" interval="0" timeout="15min"/>
     </operations>
     <instance_attributes id="params-public-ip">
        <nvpair id="public-ip-addr" name="ip" value="192.0.2.2"/>
     </instance_attributes>
   </primitive>
   -------
   =====
   
   === Multiple Monitor Operations ===
   
   Provided no two operations (for a single resource) have the same name
   and interval, you can have as many +monitor+ operations as you like.
   In this way, you can do a superficial health check every minute and
   progressively more intense ones at higher intervals.
   
   To tell the resource agent what kind of check to perform, you need to
   provide each monitor with a different value for a common parameter.
   The OCF standard creates a special parameter called +OCF_CHECK_LEVEL+
   for this purpose and dictates that it is "made available to the
   resource agent without the normal +OCF_RESKEY+ prefix".
   
   Whatever name you choose, you can specify it by adding an
   +instance_attributes+ block to the +op+ tag. It is up to each
   resource agent to look for the parameter and decide how to use it.
   
   .An OCF resource with two recurring health checks, performing different levels of checks specified via +OCF_CHECK_LEVEL+.
   =====
   [source,XML]
   -------
   <primitive id="Public-IP" class="ocf" type="IPaddr" provider="heartbeat">
      <operations>
         <op id="public-ip-health-60" name="monitor" interval="60">
            <instance_attributes id="params-public-ip-depth-60">
               <nvpair id="public-ip-depth-60" name="OCF_CHECK_LEVEL" value="10"/>
            </instance_attributes>
         </op>
         <op id="public-ip-health-300" name="monitor" interval="300">
            <instance_attributes id="params-public-ip-depth-300">
               <nvpair id="public-ip-depth-300" name="OCF_CHECK_LEVEL" value="20"/>
            </instance_attributes>
        </op>
      </operations>
      <instance_attributes id="params-public-ip">
          <nvpair id="public-ip-level" name="ip" value="192.0.2.2"/>
      </instance_attributes>
   </primitive>
   -------
   =====
   
   === Disabling a Monitor Operation ===
   
   The easiest way to stop a recurring monitor is to just delete it.
   However, there can be times when you only want to disable it
   temporarily.  In such cases, simply add +enabled=false+ to the
   operation's definition.
   
   .Example of an OCF resource with a disabled health check
   =====
   [source,XML]
   -------
   <primitive id="Public-IP" class="ocf" type="IPaddr" provider="heartbeat">
      <operations>
         <op id="public-ip-check" name="monitor" interval="60s" enabled="false"/>
      </operations>
      <instance_attributes id="params-public-ip">
         <nvpair id="public-ip-addr" name="ip" value="192.0.2.2"/>
      </instance_attributes>
   </primitive>
   -------
   =====
   
   This can be achieved from the command line by executing:
   
   ----
   # cibadmin --modify --xml-text '<op id="public-ip-check" enabled="false"/>'
   ----
   
   Once you've done whatever you needed to do, you can then re-enable it with
   ----
   # cibadmin --modify --xml-text '<op id="public-ip-check" enabled="true"/>'
   ----
